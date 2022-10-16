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


/* GLOBAL DEFINITIONS */
#define PACER_FREQUENCY 1000     // Define pacer frequency, runs n loop in one seond. Where n is the value of PACER_FREQUENCY
#define NAV_POLL_RATE 100        // Defining navswitch frequency
#define IR_POLL_RATE 100         // Defining the IR poll rate
#define MSG_SPEED 15             // Scroll speed of the message


/* DEFINING TYPES */
typedef enum {PLAYER, MISSILE} Obejct_type;     // Defining object types as either a missile or player
typedef enum {IDLE = 0, ACTIVE = 1} State;      // Defining State as either active or idle in an enum

typedef struct 
{
    Obejct_type type;
    State status;
    tinygl_point_t pos;

} game_object_t;               // Struct of the object. Contains its type, status and position         


/* OTHER VARIABLES */
static bool GAME_STATE = 0;              // The game is initially not on
const char game_name[] = "CS0.16\0";     // Name of the game which appears before the game starts
const char game_over_msg[] = "GGEZ\0";   // Loser message "GG"
const char win_msg[] = "youwin!\0";      // Winner message "You win!"
uint16_t missile_speed = 500;            // Initial speed of the missile



// FUNCTIONS ****************************************************************************************************************


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
    else {
        return 0;
    }
}


/** Gets the position of a game object through coordinates
    @param game object
    @return a point */
tinygl_point_t get_pos(game_object_t object)
{
    tinygl_point_t point;
    point.x = object.pos.x;
    point.y = object.pos.y;
    return point;
}


/** Moves the player using the navswitch
    @param pointer of the player */
void player_move(game_object_t* player_ptr) 
{
    if (navswitch_push_event_p(NAVSWITCH_NORTH)) {
        tinygl_draw_point(player_ptr->pos,0);
        if (player_ptr->pos.y != 0) {
            player_ptr->pos.y -= 1;
        }
    } 

    else if (navswitch_push_event_p(NAVSWITCH_EAST)) {
        tinygl_draw_point(player_ptr->pos,0);
        if (player_ptr->pos.x != 4) {
            player_ptr->pos.x += 1;
        }
    } 

    else if (navswitch_push_event_p(NAVSWITCH_SOUTH)) {
        tinygl_draw_point(player_ptr->pos,0);
        if (player_ptr->pos.y != 6) {
            player_ptr->pos.y += 1;
        }
    } 

    else if (navswitch_push_event_p(NAVSWITCH_WEST)) {
        tinygl_draw_point(player_ptr->pos,0);
        if (player_ptr->pos.x != 0) {
        player_ptr->pos.x -= 1;
        }
    }
    tinygl_draw_point(player_ptr->pos,1); 
}


/** Creates a player
    @return player object */
game_object_t player_create(void)
{
    tinygl_point_t start_pos = tinygl_point(4,3);
    game_object_t player = {PLAYER,ACTIVE,start_pos};
    tinygl_draw_point(player.pos,1);
    return player;
}


/** Resets the player
    @param player pointer */
void player_reset(game_object_t* player_ptr) 
{
    player_ptr->status = 1;
    player_ptr->pos =  tinygl_point(4,3);
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


/** Creates a missile
    @return a missile object type */
game_object_t missile_create(void)
{
    tinygl_point_t pos = {0,0};
    game_object_t missile = {MISSILE, IDLE, pos};
    return missile;
}


/** Lauches the missile
    @param pointer of the missile
    @param position of the missile */
void missile_launch(game_object_t* missile_ptr, tinygl_point_t pos)
{
    missile_ptr->status = 1;
    missile_ptr->pos = pos;
    missile_ptr->pos.x -= 1;
    tinygl_draw_point(missile_ptr->pos,1);
}


/** Sends a missile to other funkit
    uses flipped_y to account for the funkit orientation
    @param y-coordinate of the missile */
void missile_send(tinygl_coord_t y)
{
    int8_t flipped_y = (y - 6) * -1;
    char ch = 48 + flipped_y;
    ir_uart_putc(ch);
}


/** Updates the missile 
    @param Missile pointer */
void missile_update(game_object_t* missile_ptr)
{
    tinygl_draw_point(missile_ptr->pos,0);
    missile_ptr->pos.x -= 1;
    tinygl_draw_point(missile_ptr->pos,1);
    if (missile_ptr->pos.x < 0) 
    {
        missile_ptr->status = 0;
        missile_send(missile_ptr->pos.y);
    }
}


/** Gets the position of a game object through coordinates
    @param game object
    @return a point */
void incoming_missile_update(game_object_t* incoming_missile_ptr, game_object_t* player_ptr)
{
    tinygl_draw_point(incoming_missile_ptr->pos,0);
    incoming_missile_ptr->pos.x += 1;
    tinygl_draw_point(incoming_missile_ptr->pos,1);
    if (check_hit(player_ptr, incoming_missile_ptr))
    {
        game_over();
    }
    if (incoming_missile_ptr->pos.x > 4) 
    {
        incoming_missile_ptr->status = 0;
    }
}



// MAIN *************************************************************************************************************************


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
                        if (msg >= 48 && msg < 55) {
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
            if (incoming_missile.status == 1)
            {
                if (incoming_missile_tick >= missile_speed)
                {   
                    incoming_missile_tick = 0;
                    incoming_missile_update(&incoming_missile, &player);
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
