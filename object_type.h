    /**  @file object_type.h
         @author John Chan Jack Barton
         @date 10th October 2022
         @brief Object type (missile or player) header file
    */
   

#ifndef OBJECT_TYPE_H
#define OBJECT_TYPE_H

#include "tinygl.h"
#include "system.h"


/* Defining object types as either a missile or player */
typedef enum {PLAYER, MISSILE} Obejct_type;


/* Defining State as either active or idle in an enum */
typedef enum {IDLE = 0, ACTIVE = 1} State;


/* Struct of the object. Contains its type, status and position */
typedef struct 
{
    Obejct_type type;
    State status;
    tinygl_point_t pos;

} game_object_t;


/** Gets the position of a game object through coordinates
    @param game object
    @return a point */
tinygl_point_t get_pos(game_object_t object);


#endif