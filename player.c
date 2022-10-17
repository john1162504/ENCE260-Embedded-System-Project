    /**  @file player.c
         @author John Chan Jack Barton
         @date 10th October 2022
         @brief Player-related functions
    */

#include "tinygl.h"
#include "system.h"
#include "object_type.h"
#include "navswitch.h"
#include "player.h"


/** Moves the player using the navswitch
    @param pointer of the player */
void player_move(game_object_t* player_ptr) 
{
    if (navswitch_push_event_p(NAVSWITCH_NORTH)) {
        tinygl_draw_point(player_ptr->pos,0);
        if (player_ptr->pos.y != 0) {
            player_ptr->pos.y -= 1;
        }
    } 

    else if (navswitch_push_event_p(NAVSWITCH_EAST)) {
        tinygl_draw_point(player_ptr->pos,0);
        if (player_ptr->pos.x != 4) {
            player_ptr->pos.x += 1;
        }
    } 

    else if (navswitch_push_event_p(NAVSWITCH_SOUTH)) {
        tinygl_draw_point(player_ptr->pos,0);
        if (player_ptr->pos.y != 6) {
            player_ptr->pos.y += 1;
        }
    } 

    else if (navswitch_push_event_p(NAVSWITCH_WEST)) {
        tinygl_draw_point(player_ptr->pos,0);
        if (player_ptr->pos.x != 0) {
        player_ptr->pos.x -= 1;
        }
    }
    tinygl_draw_point(player_ptr->pos,1); 
}


/** Creates a player
    @return player object */
game_object_t player_create(void)
{
    tinygl_point_t start_pos = tinygl_point(4,3);
    game_object_t player = {PLAYER,ACTIVE,start_pos};
    tinygl_draw_point(player.pos,1);
    return player;
}


/** Resets the player
    @param player pointer */
void player_reset(game_object_t* player_ptr) 
{
    player_ptr->status = 1;
    player_ptr->pos =  tinygl_point(4,3);
}