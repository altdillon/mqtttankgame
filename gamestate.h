#ifndef GAMESTATE_H
#define GAMESTATE_H
#include <MQTTClient.h>

typedef struct 
{
    Vector2 spritePos;
    float angle; // angle in radians
    uint16_t bullets; // how many bullets the tank has
    // TODO: add a type for ammo type
} player_t;

typedef struct 
{
    player_t *player;
    bool isHosts_loaded;
    uint32_t loaded_hosts;
} gamestate_t;

#endif