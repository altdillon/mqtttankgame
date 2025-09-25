#ifndef BADGUY_H
#define BADGUY_H
#define MAXBADGUYTANKS 20 // max number of bad guy tanks
#include <stdint.h>
#include "raylib.h"
#include "mqtthosts.h"

/*
    enum for the operating state of the bad guy tank
*/
enum tankstate
{
    PATROL, // figure out how far the player is from the cpu, act based on that distance
    SEARCH, // move the tank to face the player 
    FIRE, // shoot of a round, then move
    RETREAT, // if it, or we just feel like it retreat
    MOVE // pick a direction to move in
};

/*
    struct that stores the state of the current tank
*/
typedef struct 
{
    Vector2 tankpos; // 2D position of the badguy tank
    enum tankstate currentstate;
} badguytank_h;

/*
    header prototypes for the bad guy tank
*/

/*
    init the tank, start out each tank on patrol
    mqtthosts: array of mqtt hosts from the toml file
    badguys: array of badguy tanks
    ntanks: number of tanks that we want to create
    return 0 if everything word out, -1 if it didn't
*/
int init_tanks(mqtthost_t *mqtthosts,badguytank_h *badguys,uint32_t ntanks);

/*
    compute the next state of a tank and update the memory
    tank: a pointer to a tank object
*/

void tank_nextate(badguytank_h *tank);

#endif