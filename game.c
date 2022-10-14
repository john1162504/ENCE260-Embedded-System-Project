    /**  @file game.c
         @author John Chan Jack Barton
         @date 10th October 2022
         @brief Connect four game

        @defgroup Two player connect four game
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

/** Move gun/player when navswitch push */
// void player_move() 
// {
//     if (navswitch_push_event_p(NAVSWITCH_NORTH)) {
//         tinygl_draw_point(player,0);
//         player.y -= 1;
//     } 
    
//     else if (navswitch_push_event_p(NAVSWITCH_EAST)) {
//         tinygl_draw_point(player,0);
//         player.x += 1;
//     } 
    
//     else if (navswitch_push_event_p(NAVSWITCH_SOUTH)) {
//         tinygl_draw_point(player,0);
//         player.y += 1;
//     } 
    
//     else if (navswitch_push_event_p(NAVSWITCH_WEST)) {
//         tinygl_draw_point(player,0);
//         player.x -= 1;
//     }
    
//     tinygl_draw_point(player,1); 
// }

// /** Display string when game over TBD */
// void game_over()
// {

// }


tinygl_point_t get_pos(tinygl_point_t object)
{
    tinygl_point_t new_obj;
    new_obj.x = object.x;
    new_obj.y = object.y;
    return new_obj;
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
void missile_send(tinygl_coord_t x)
{
    char ch = 48 + x;
    ir_uart_putc(ch);
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
    tinygl_init(1000);
    ir_uart_init();
    tinygl_point_t player = tinygl_point(4,3);
    game_object_t missile = missile_initiate();
    game_object_t incoming_missile = missile_initiate();
    //initiate_barriers();
    tinygl_draw_point(player,1);

    uint16_t navswitch_ticks = 0;
    uint16_t missile_tick = 0;
    uint16_t ir_read_tick = 0;
    uint16_t incoming_missile_tick = 0;






    while (1)
    {
        pacer_wait();
        tinygl_update();
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
                else {
                    continue;
                }
            } else {
                if (navswitch_push_event_p(NAVSWITCH_NORTH)) {
                    tinygl_draw_point(player,0);
                    player.y -= 1;
                } 

                else if (navswitch_push_event_p(NAVSWITCH_EAST)) {
                    tinygl_draw_point(player,0);
                    player.x += 1;
                } 

                else if (navswitch_push_event_p(NAVSWITCH_SOUTH)) {
                    tinygl_draw_point(player,0);
                    player.y += 1;
                } 

                else if (navswitch_push_event_p(NAVSWITCH_WEST)) {
                    tinygl_draw_point(player,0);
                    player.x -= 1;
                }

                tinygl_draw_point(player,1); 
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
                char num = ir_uart_getc();
                if (num >= 48 && num < 55) {
                    incoming_missile.status = 1;
                    incoming_missile.pos = tinygl_point(5,num);
                    tinygl_draw_point(missile.pos,1);
                    incoming_missile_tick = 0;
                }

            }
        }
        if (incoming_missile_tick > 500)
        {
            incoming_missile_tick = 0;
            if (incoming_missile.status == 1)
            {   
                tinygl_draw_point(incoming_missile.pos,0);
                incoming_missile.pos.x += 1;
                tinygl_draw_point(incoming_missile.pos,1);
                if (incoming_missile.pos.x > 5) 
                {
                    incoming_missile.status = 0;
                }
            }
        }
    }
}
