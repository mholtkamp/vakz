
#ifndef CONSTANTS_H
#define CONSTANTS_H

#define GAME_STATE_MENU 0
#define GAME_STATE_GAME 1

#define PLAYER_USER_SIZE 15
#define PLAYER_PASS_SIZE 15
#define USER_BUFFER_SIZE 16
#define PASS_BUFFER_SIZE 16
#define HAND_SIZE 5
#define DECK_SIZE 30
#define COLLECTION_SIZE 512

#define LOGIN_STATUS_NONE                 0
#define LOGIN_STATUS_OK                   1
#define LOGIN_STATUS_INVALID_PASS         2
#define LOGIN_STATUS_USER_NOT_FOUND       3
#define LOGIN_STATUS_REGISTRATION_FAILED  4

#define GAME_STATE_INACTIVE 0
#define GAME_STATE_WAITING  1
#define GAME_STATE_ACTIVE   2
#define GAME_STATE_OVER     3

#define QUEUE_TYPE_NONE 0
#define QUEUE_TYPE_SOLO 1

#define QUEUE_STATUS_NONE        0
#define QUEUE_STATUS_MATCH_FOUND 1
#define QUEUE_STATUS_FULL        2
#define QUEUE_STATUS_ERROR       3

#define GRID_WIDTH  6
#define GRID_HEIGHT 3

#define TILE_WIDTH  1.5f
#define TILE_HEIGHT 1.0f

#define TILE_TYPE_NORMAL      0
#define TILE_TYPE_CRACKED     1
#define TILE_TYPE_EMPTY       2
#define TILE_TYPE_LAVA        3
#define TILE_TYPE_ICE         4
#define TILE_TYPE_HEAL        5

#define SIDE_1 0
#define SIDE_2 1

#define MAGE_SCALE_XYZ 1.75f
#define MAGE_Y_OFFSET 0.5f

#define DEFAULT_MAGE_HEALTH 500

#define RARITY_COMMON   0
#define RARITY_UNCOMMON 1
#define RARITY_RARE     2
#define RARITY_EPIC     3

#endif