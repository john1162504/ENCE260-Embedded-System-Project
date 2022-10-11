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


/** Config led matrix */
void ledmat_init(void)
{
    uint8_t row;
    uint8_t col;

    for (row = 0; row < NUM_OF_ROW; row++)
    {
        pio_config_set (ledmat_rows[row], PIO_OUTPUT_HIGH);
        pio_output_high (ledmat_rows[row]);
    }

    for (col = 0; col < NUM_OF_COL; col++)
    {
        pio_config_set (ledmat_cols[col], PIO_OUTPUT_HIGH);
        pio_output_high (ledmat_cols[col]);
    }
}

/** Initiate barriers at the start of the game */
void initiate_barriers()
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


/** Move gun/player when navswitch push */
void gun_move() 
{

}

/** Display string when game over TBD */
void game_over()
{

}

/** Actions when missile hit an object(player or barrier, 
     missiles will not collide as it would make the game too long)
     TBD */
void missile_hit()
{

}




int main(void)
{
    system_init();
    pacer_init(1000);
    ledmat_init();




    while (1)
    {
        peacer_wait();




    }
}
