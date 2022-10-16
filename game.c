    /**  @file game.c
         @author John Chan Jack Barton
         @date 10th October 2022
         @brief CS 0.16

        @defgroup Two player shooting game
    */



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


/** Define pacer frequency, runs n loop in one seond. Where n is the value of PACER_FREQUENCY */
#define PACER_FREQUENCY 1000
#define NAV_POLL_RATE 100
#define IR_POLL_RATE 100
#define MSG_SPEED 15

/** Define number of rows and columns */

typedef enum {PLAYER, MISSILE} Obejct_type;
typedef enum {IDLE = 0, ACTIVE = 1} State;

typedef struct 
{
    Obejct_type type;
    State status;
    tinygl_point_t pos;

} game_object_t;

static bool GAME_STATE = 0;
const char game_name[] = "CS0.16\0";
const char game_over_msg[] = "GG\0";
const char win_msg[] = "you win!\0";
uint16_t missile_speed = 500;





bool check_hit(game_object_t* player, game_object_t* incoming_missile)
{
    if (player->pos.x == incoming_missile->pos.x && player->pos.y == incoming_missile->pos.y)
    {
        player->status = 0;
        tinygl_draw_point(player->pos, 0);
        tinygl_draw_point(incoming_missile->pos, 0);
        incoming_missile->status = 0;
        return 1;
    }
    else {
        return 0;
    }
}

tinygl_point_t get_pos(game_object_t object)
{
    tinygl_point_t point;
    point.x = object.pos.x;
    point.y = object.pos.y;
    return point;
}



/** Move gun/player when navswitch push */
void player_move(game_object_t* player) 
{
    if (navswitch_push_event_p(NAVSWITCH_NORTH)) {
        tinygl_draw_point(player->pos,0);
        if (player->pos.y != 0) {
            player->pos.y -= 1;
        }
    } 

    else if (navswitch_push_event_p(NAVSWITCH_EAST)) {
        tinygl_draw_point(player->pos,0);
        if (player->pos.x != 4) {
            player->pos.x += 1;
        }
    } 

    else if (navswitch_push_event_p(NAVSWITCH_SOUTH)) {
        tinygl_draw_point(player->pos,0);
        if (player->pos.y != 6) {
            player->pos.y += 1;
        }
    } 

    else if (navswitch_push_event_p(NAVSWITCH_WEST)) {
        tinygl_draw_point(player->pos,0);
        if (player->pos.x != 0) {
        player->pos.x -= 1;
        }
    }
    tinygl_draw_point(player->pos,1); 
}


game_object_t player_create(void)
{
    tinygl_point_t start_pos = tinygl_point(4,3);
    game_object_t player = {PLAYER,ACTIVE,start_pos};
    tinygl_draw_point(player.pos,1);
    return player;
}

void player_reset(game_object_t* player) 
{
    player->status = 1;
    player->pos =  tinygl_point(4,3);
}


void game_start(game_object_t* player_ptr)
{
    tinygl_clear();
    player_reset(player_ptr);
    GAME_STATE = 1;
}

void game_over(void)
{
    missile_speed = 500;
    GAME_STATE = 0;
    tinygl_text(game_over_msg);
    ir_uart_putc('w');
}




game_object_t missile_create(void)
{
    tinygl_point_t pos = {0,0};
    game_object_t missile = {MISSILE, IDLE, pos};
    return missile;
}

void missile_launch(game_object_t* missile_ptr, tinygl_point_t pos)
{
    missile_ptr->status = 1;
    missile_ptr->pos = pos;
    missile_ptr->pos.x -= 1;
    tinygl_draw_point(missile_ptr->pos,1);
}

/** Take the y coordinate and send a char object repersent the coordinate of y */
void missile_send(tinygl_coord_t y)
{
    int8_t i = y - 6;
    if (i < 0)
    {
        i *= -1;
    }
    int8_t coord = i;
    char ch = 48 + coord;
    ir_uart_putc(ch);
}

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














int main(void)
{
    system_init();
    pacer_init(PACER_FREQUENCY);
    ledmat_init();
    navswitch_init();
    ir_uart_init();



    tinygl_init(PACER_FREQUENCY);
    tinygl_font_set(&font5x5_1);
    tinygl_text_speed_set(MSG_SPEED);
    tinygl_text_mode_set (TINYGL_TEXT_MODE_SCROLL);
    tinygl_text_dir_set(TINYGL_TEXT_DIR_ROTATE);
    tinygl_text(game_name);


    
    game_object_t player = player_create();
    game_object_t missile = missile_create();
    game_object_t incoming_missile = missile_create();



    uint16_t navswitch_ticks = 0;
    uint16_t missile_tick = 0;
    uint16_t ir_read_tick = 0;
    uint16_t incoming_missile_tick = 0;
    uint16_t loop_count = 0;





    while (1)
    {

        pacer_wait();
        tinygl_update();
        if (!GAME_STATE)
        {
            navswitch_update();
            if (navswitch_push_event_p(NAVSWITCH_PUSH))
            {
                game_start(&player);
            }
                
        }
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
                if (navswitch_push_event_p(NAVSWITCH_PUSH))
                {
                    if (missile.status == 0) 
                    {
                    missile_tick = 0;
                    missile_launch(&missile,get_pos(player));
                    }
                }
                else 
                {
                    if (player.status == 1) 
                    {
                    player_move(&player); 
                    }
                }

            }
            if (missile_tick >= missile_speed)
            {
                missile_tick = 0;
                if (missile.status == 1)
                {   
                    missile_update(&missile);
                }
            }
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
            if (incoming_missile.status == 1)
            {
                if (incoming_missile_tick >= missile_speed)
                {   
                    incoming_missile_tick = 0;
                    incoming_missile_update(&incoming_missile, &player);
                }
            }
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
