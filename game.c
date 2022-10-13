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


/** Define pacer frequency, runs n loop in one seond. Where n is the value of PACER_FREQUENCY */
#define PACER_FREQUENCY 1000

/** Define number of rows and columns */
#define NUM_OF_ROW 7
#define NUM_OF_COL 5

/** Define 2D array to store occupied tokens with respect of player 1 and player2
 0 means avilible
 1 means occupied
*/

uint8_t player1 [NUM_OF_ROW][NUM_OF_COL] = {0};
uint8_t player2 [NUM_OF_ROW][NUM_OF_COL] = {0};


/** Define PIO pins for led matrix */
static pio_t rows[] =
{
    LEDMAT_ROW1_PIO, LEDMAT_ROW2_PIO, LEDMAT_ROW3_PIO, LEDMAT_ROW4_PIO,
    LEDMAT_ROW5_PIO, LEDMAT_ROW6_PIO, LEDMAT_ROW7_PIO
};
static pio_t cols[] =
{
    LEDMAT_COL1_PIO, LEDMAT_COL2_PIO, LEDMAT_COL3_PIO,
    LEDMAT_COL4_PIO, LEDMAT_COL5_PIO
};



/** Initiate barriers at the start of the game */
void initiate_barriers()
{  
    tinygl_point_t b1 = tinygl_point(1,0);
    tinygl_point_t b2 = tinygl_point(1,2);
    tinygl_point_t b3 = tinygl_point(1,4);
    tinygl_point_t b4 = tinygl_point(1,6);
    tinygl_point_t b5 = tinygl_point(3,2);
    tinygl_point_t b6 = tinygl_point(3,4);
    tinygl_draw_line(b1,b2,1);
    tinygl_draw_line(b3,b4,1);
    tinygl_draw_line(b5,b6,1);
}

/** Turn off barrier when hit */

void destory_barrier(uint8_t x_pos, uint8_t y_pos)
{

}

/** Launch missile when callled*/
void missile_launch(uint8_t row, uint8_t col)
{

}

/** Actions after missile "sinal" received from other device TBD */
void missile_receive()
{

} 

/** Actions when missile hit an object(player or barrier, 
     missiles will not collide as it would make the game too long)
     TBD */
void missile_hit()
{

}

void display_player()
{

}

/** Move gun/player when navswitch push */
void player_move() 
{

}

/** Display string when game over TBD */
void game_over()
{

}






int main(void)
{
    system_init();
    pacer_init(PACER_FREQUENCY);
    ledmat_init();
    navswitch_init();
    tinygl_init(1000);
    tinygl_point_t player = tinygl_point(4,3);
    //initiate_barriers();
    tinygl_draw_point(player,1);

    uint16_t navswitch_ticks = 0;









    while (1)
    {
        pacer_wait();
        tinygl_update();
        navswitch_ticks++;
        
        if (navswitch_ticks > 50)
            {
                navswitch_ticks = 0;
                navswitch_update();

                if (navswitch_push_event_p(NAVSWITCH_NORTH))
                {
                    tinygl_draw_point(player,0);
                    player.y -= 1;
                } else if (navswitch_push_event_p(NAVSWITCH_EAST))
                {
                    tinygl_draw_point(player,0);
                    player.x += 1;
                } else if (navswitch_push_event_p(NAVSWITCH_SOUTH))
                {
                    tinygl_draw_point(player,0);
                    player.y += 1;
                } else if (navswitch_push_event_p(NAVSWITCH_WEST))
                {
                    tinygl_draw_point(player,0);
                    player.x -= 1;
                }
                tinygl_draw_point(player,1);

            }

    }
}
