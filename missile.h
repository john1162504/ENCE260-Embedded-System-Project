    /**  @file missile.h
         @author John Chan Jack Barton
         @date 10th October 2022
         @brief Missile header file
    */
   

#ifndef MISSILE_H
#define MISSILE_H

#include "tinygl.h"
#include "system.h"
#include "object_type.h"
#include "ir_uart.h"


/** Creates a missile
    @return a missile object type */
game_object_t missile_create(void);


/** Launches the missile
    @param pointer of the missile
    @param position of the missile */
void missile_launch(game_object_t* missile_ptr, tinygl_point_t pos);


/** Sends a missile to other funkit
    uses flipped_y to account for the funkit orientation
    @param y-coordinate of the missile */
void missile_send(tinygl_coord_t y);


/** Updates the missile 
    @param Missile pointer */
void missile_update(game_object_t* missile_ptr);


/** Gets the position of a game object through coordinates
    @param game object
    @return a point */
void incoming_missile_update(game_object_t* incoming_missile_ptr);


#endif