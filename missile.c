    /**  @file missile.c
         @author John Chan Jack Barton
         @date 10th October 2022
         @brief Missile-related functions
    */

#include "tinygl.h"
#include "system.h"
#include "object_type.h"
#include "ir_uart.h"
#include "missile.h"


/** Creates a missile
    @return a missile object type */
game_object_t missile_create(void)
{
    tinygl_point_t pos = {0,0};
    game_object_t missile = {MISSILE, IDLE, pos};
    return missile;
}


/** Launches the missile
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
void incoming_missile_update(game_object_t* incoming_missile_ptr)
{
    tinygl_draw_point(incoming_missile_ptr->pos,0);
    incoming_missile_ptr->pos.x += 1;
    tinygl_draw_point(incoming_missile_ptr->pos,1);
}