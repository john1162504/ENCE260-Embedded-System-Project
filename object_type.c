    /**  @file object_type.c
         @author John Chan Jack Barton
         @date 10th October 2022
         @brief Defining the object_type
    */

#include "tinygl.h"
#include "system.h"
#include "object_type.h"


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