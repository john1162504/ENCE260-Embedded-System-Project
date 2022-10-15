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

/** Define number of rows and columns */
#define NUM_OF_ROW 7
#define NUM_OF_COL 5
typedef enum {PLAYER, BARRIER, MISSILE} Obejct_type;
typedef enum {IDLE = 0, ACTIVE = 1} State;

typedef struct 
{
    Obejct_type type;
    State status;
    tinygl_point_t pos;

} game_object_t;

static bool GAME_STATE = 0;
const char game_name[] = "CS0.16";
const char game_over_msg[] = "GG";







// /** Define PIO pins for led matrix */
// static pio_t rows[] =
// {
//     LEDMAT_ROW1_PIO, LEDMAT_ROW2_PIO, LEDMAT_ROW3_PIO, LEDMAT_ROW4_PIO,
//     LEDMAT_ROW5_PIO, LEDMAT_ROW6_PIO, LEDMAT_ROW7_PIO
// };
// static pio_t cols[] =
// {
//     LEDMAT_COL1_PIO, LEDMAT_COL2_PIO, LEDMAT_COL3_PIO,
//     LEDMAT_COL4_PIO, LEDMAT_COL5_PIO
// };



// /** Initiate barriers at the start of the game */
// void initiate_barriers(void)
// {  
//     tinygl_point_t b1 = tinygl_point(1,0);
//     tinygl_point_t b2 = tinygl_point(1,2);
//     tinygl_point_t b3 = tinygl_point(1,4);
//     tinygl_point_t b4 = tinygl_point(1,6);
//     tinygl_point_t b5 = tinygl_point(3,2);
//     tinygl_point_t b6 = tinygl_point(3,4);
//     tinygl_draw_line(b1,b2,1);
//     tinygl_draw_line(b3,b4,1);
//     tinygl_draw_line(b5,b6,1);
// }

// /** Turn off barrier when hit */

// void destory_barrier(uint8_t x_pos, uint8_t y_pos)
// {

// }

// /** Launch missile when callled*/
// void missile_launch(uint8_t row, uint8_t col)
// {

// }

// /** Actions after missile "sinal" received from other device TBD */
// void missile_receive()
// {

// } 

// /** Actions when missile hit an object(player or barrier, 
//      missiles will not collide as it would make the game too long)
//      TBD */
// void missile_hit()
// {

// }

// void missile_update(game_object_t missile)
// {

//     missile.pos.x -= 1;
//     tinygl_draw_point(missile.pos,1);

// }

// void display_player()
// {

// }

void game_start_screen(void)
{
    
}


void game_finsih_screen(void)
{

}

/** Move gun/player when navswitch push */
void player_move(game_object_t* player) 
{
    if (navswitch_push_event_p(NAVSWITCH_NORTH)) {
        tinygl_draw_point(player->pos,0);
        player->pos.y -= 1;
    } 

    else if (navswitch_push_event_p(NAVSWITCH_EAST)) {
        tinygl_draw_point(player->pos,0);
        player->pos.x += 1;
    } 

    else if (navswitch_push_event_p(NAVSWITCH_SOUTH)) {
        tinygl_draw_point(player->pos,0);
        player->pos.y += 1;
    } 

    else if (navswitch_push_event_p(NAVSWITCH_WEST)) {
        tinygl_draw_point(player->pos,0);
        player->pos.x -= 1;
    }
    tinygl_draw_point(player->pos,1); 
}

// /** Display string when game over TBD */
// void game_over()
// {

// }


tinygl_point_t get_pos(game_object_t object)
{
    tinygl_point_t point;
    point.x = object.pos.x;
    point.y = object.pos.y;
    return point;
}

game_object_t missile_initiate(void)
{
    tinygl_point_t pos = {0,0};
    game_object_t missile = {MISSILE, IDLE, pos};
    return missile;
}

void missile_update(game_object_t missile)
{
    tinygl_draw_point(missile.pos,0);
    missile.pos.x -= 1;
    tinygl_draw_point(missile.pos,1);
}

/** Take the x coordinate and send a char object repersent the coordinate of x */
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


game_object_t player_initiate(void)
{
    tinygl_point_t start_pos = tinygl_point(4,3);
    game_object_t player = {PLAYER,ACTIVE,start_pos};
    tinygl_draw_point(player.pos,1);
    return player;
}

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



// void missile_receive()
// {

// }



int main(void)
{
    system_init();
    pacer_init(PACER_FREQUENCY);
    ledmat_init();
    navswitch_init();
    char* display_msg = &game_name;
    tinygl_init(1000);
    tinygl_font_set(&font5x5_1);
    tinygl_text_speed_set(10);
    tinygl_text_mode_set (TINYGL_TEXT_MODE_SCROLL);
    tinygl_text_dir_set(TINYGL_TEXT_DIR_ROTATE);
    tinygl_text(display_msg);


    
    ir_uart_init();
    game_object_t player = player_initiate();
    game_object_t missile = missile_initiate();
    game_object_t incoming_missile = missile_initiate();
    //initiate_barriers();
    uint16_t navswitch_ticks = 0;
    uint16_t missile_tick = 0;
    uint16_t ir_read_tick = 0;
    uint16_t incoming_missile_tick = 0;


    // while (1)
    // {
    //     pacer_wait();
    //     tinygl_update();
    //     navswitch_update();
    //     if (navswitch_push_event_p(NAVSWITCH_PUSH))
    //         {
    //             tinygl_clear();
    //         }

    // }



    while (1)
    {

        pacer_wait();
        tinygl_update();
        if (!GAME_STATE)
            {
            navswitch_update();

            if (navswitch_push_event_p(NAVSWITCH_PUSH))
                {
                    tinygl_clear();
                    GAME_STATE = 1;
                }
                
            }
        else {
        navswitch_ticks++;
        missile_tick++;
        ir_read_tick++;
        incoming_missile_tick++;

        if (navswitch_ticks > 50)
        {
            navswitch_ticks = 0;
            navswitch_update();
            if (navswitch_push_event_p(NAVSWITCH_PUSH))
            {
                if (missile.status == 0) {
                missile.status = 1;
                missile.pos = get_pos(player);
                missile.pos.x -= 1;
                tinygl_draw_point(missile.pos,1);
                missile_tick = 0;
                }
            } else {
                if (player.status == 1) 
                {
                player_move(&player); 
                }
            }

        }
        if (missile_tick > 500)
        {
            missile_tick = 0;
            if (missile.status == 1)
            {   
                tinygl_draw_point(missile.pos,0);
                missile.pos.x -= 1;
                tinygl_draw_point(missile.pos,1);
                if (missile.pos.x < 0) 
                {
                    missile.status = 0;
                    missile_send(missile.pos.y);
                }
            }
        }
        if (ir_read_tick > 100)
        {
            if (ir_uart_read_ready_p())
            {
                char ch = ir_uart_getc();
                if (ch >= 48 && ch < 55) {
                    int num = ch - '0';
                    incoming_missile.status = 1;
                    incoming_missile.pos = tinygl_point(0,num);
                    tinygl_draw_point(incoming_missile.pos,1);
                    incoming_missile_tick = 0;
                }

            }
        }
        if (incoming_missile.status == 1)
        {
            if (incoming_missile_tick > 500)
            {   
                incoming_missile_tick = 0;
                tinygl_draw_point(incoming_missile.pos,0);
                incoming_missile.pos.x += 1;
                tinygl_draw_point(incoming_missile.pos,1);
                if (check_hit(&player, &incoming_missile))
                {
                    GAME_STATE = 0;
                    display_msg = &game_over_msg;
                }
                if (incoming_missile.pos.x < 0) 
                {
                    incoming_missile.status = 0;
                }
            }
        }
        }
    }
}
