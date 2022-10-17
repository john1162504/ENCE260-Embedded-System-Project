    /**  @file game.c
         @author John Chan Jack Barton
         @date 10th October 2022
         @brief CS 0.16

        @defgroup Two player shooting game
    */


// INITIALISATIONS ***********************************************************************************************************

#include "system.h"
#include "pio.h"
#include "led.h"
#include "navswitch.h"
#include "pacer.h"
#include "ledmat.h"
#include "display.h"
#include "tinygl.h"
#include "ir_uart.h"
#include "../fonts/font5x5_1.h"
#include "object_type.h"
#include "missile.h"
#include "player.h"


/* GLOBAL DEFINITIONS */
#define PACER_FREQUENCY 1000     // Define pacer frequency, runs n loop in one seond. Where n is the value of PACER_FREQUENCY
#define NAV_POLL_RATE 100        // Defining navswitch frequency
#define IR_POLL_RATE 100         // Defining the IR poll rate
#define MSG_SPEED 15             // Scroll speed of the message       


/* OTHER VARIABLES */
static bool GAME_STATE = 0;              // The game is initially not on
const char game_name[] = "CS0.16\0";     // Name of the game which appears before the game starts
const char game_over_msg[] = "GGEZ\0";   // Loser message "GG"
const char win_msg[] = "youwin!\0";      // Winner message "You win!"
uint16_t missile_speed = 500;            // Initial speed of the missile



// GAME FUNCTIONS ************************************************************************************************************


/** Checks whether a missile hits the player
    @param pointer of the player
    @param pointer of the missile object
    @return bool of whether the missile hits */
bool check_hit(game_object_t* player_ptr, game_object_t* incoming_missile_ptr)
{
    if (player_ptr->pos.x == incoming_missile_ptr->pos.x && player_ptr->pos.y == incoming_missile_ptr->pos.y)
    {
        player_ptr->status = 0;
        tinygl_draw_point(player_ptr->pos, 0);
        tinygl_draw_point(incoming_missile_ptr->pos, 0);
        incoming_missile_ptr->status = 0;
        return 1;
    }
    else 
    {
        return 0;
    }
}


/** Starts the game
    @param player pointer */
void game_start(game_object_t* player_ptr)
{
    tinygl_clear();
    player_reset(player_ptr);
    GAME_STATE = 1;
}


/** Ends the game when the player gets killed by a missile */
void game_over(void)
{
    missile_speed = 500;
    GAME_STATE = 0;
    tinygl_text(game_over_msg);
    ir_uart_putc('w');
}



// MAIN ***********************************************************************************************************************


int main(void)
{
    /* Initialising parts of the funkit for the main loop */
    system_init();
    pacer_init(PACER_FREQUENCY);
    ledmat_init();
    navswitch_init();
    ir_uart_init();


    /* Initialisation for the text to scroll */
    tinygl_init(PACER_FREQUENCY);
    tinygl_font_set(&font5x5_1);
    tinygl_text_speed_set(MSG_SPEED);
    tinygl_text_mode_set (TINYGL_TEXT_MODE_SCROLL);
    tinygl_text_dir_set(TINYGL_TEXT_DIR_ROTATE);
    tinygl_text(game_name);


    /* Creating the player and missile objects */
    game_object_t player = player_create();
    game_object_t missile = missile_create();
    game_object_t incoming_missile = missile_create();


    /* Initialisationb of ticks to increase through the loop */
    uint16_t navswitch_ticks = 0;
    uint16_t missile_tick = 0;
    uint16_t ir_read_tick = 0;
    uint16_t incoming_missile_tick = 0;
    uint16_t loop_count = 0;


    /* Main loop */
    while (1)
    {
        pacer_wait();
        tinygl_update();
        
        /* Starts the game when the navswitch is pushed */
        if (!GAME_STATE)
        {
            navswitch_update();
            if (navswitch_push_event_p(NAVSWITCH_PUSH))
            {
                game_start(&player);
            }
                
        }
        
        /* If not, the game is underway */
        else 
        {
            navswitch_ticks++;
            missile_tick++;
            ir_read_tick++;
            incoming_missile_tick++;
            loop_count++;

            
            if (navswitch_ticks >= NAV_POLL_RATE)
            {
                navswitch_ticks = 0;
                navswitch_update();
                
                /* Shoots a missile if the navswitch is pushed */
                if (navswitch_push_event_p(NAVSWITCH_PUSH))
                {
                    if (missile.status == 0) 
                    {
                        missile_tick = 0;
                        missile_launch(&missile,get_pos(player));
                    }
                }
                
                else 
                {   /* Moves the player according to the navswitch */
                    if (player.status == 1) 
                    {
                        player_move(&player);   
                    }
                }

            }
            
            /* Shoots the missile at the desired speed */
            if (missile_tick >= missile_speed)
            {
                missile_tick = 0;
                if (missile.status == 1)
                {   
                    missile_update(&missile);
                }
            }
            
            /* Sends the missile using IR transmission */
            if (ir_read_tick >= IR_POLL_RATE)
            {
                ir_read_tick = 0;
                
                if (incoming_missile.status == 0)
                {
                    if (ir_uart_read_ready_p()) 
                    {
                        char msg = ir_uart_getc();
                        if (msg >= 48 && msg < 55) 
                        {
                            int num = msg - '0';
                            incoming_missile.status = 1;
                            incoming_missile.pos = tinygl_point(0,num);
                            tinygl_draw_point(incoming_missile.pos,1);
                            
                            if (check_hit(&player, &incoming_missile))
                                {
                                    game_over();
                                }
                            incoming_missile_tick = 0;
                        }
                        
                        if (msg == 'w')
                        {
                            tinygl_text(win_msg);
                            GAME_STATE = 0;
                            missile_speed = 500;
                        }

                    }
                }
            }
            
            /* Updates the missile position */
            if (incoming_missile_tick >= missile_speed)
            {
                if (incoming_missile.status == 1)
                {   
                    incoming_missile_tick = 0;
                    incoming_missile_update(&incoming_missile);
                    if (check_hit(&player, &incoming_missile))
                    {
                        game_over();
                    }
                    if (incoming_missile.pos.x > 4) 
                    {
                        incoming_missile.status = 0;
                    }
                }
            }
            
            /* Increases the missile speed with time */ 
            if (loop_count >= PACER_FREQUENCY)
            {
                loop_count = 0;
                if ((missile_speed - 25) > 50 )
                {
                    missile_speed -= 25;
                }
            }
        }
    }
}