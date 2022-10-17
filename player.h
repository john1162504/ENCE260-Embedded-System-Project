    /**  @file player.h
         @author John Chan Jack Barton
         @date 10th October 2022
         @brief Player header file
    */
   

#ifndef PLAYER_H
#define PLAYER_H

#include "tinygl.h"
#include "system.h"
#include "object_type.h"
#include "navswitch.h"


/* Moves the player using the navswitch */
void player_move(game_object_t* player_ptr);


/* Creates a player */
game_object_t player_create(void);


/* Resets the player */
void player_reset(game_object_t* player_ptr) ;


#endif