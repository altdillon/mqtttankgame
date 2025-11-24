#ifndef BADGUY_H
#define BADGUY_H
#define MAXBADGUYTANKS 80 // max number of bad guy tanks
#define MAXTANKHEALTH 100
#include <stdint.h>
#include <stdbool.h>
#include "raylib.h"
#include "mqtthosts.h"
#include "gamestate.h"
#include "bullets.h"

/*
    enum for the operating state of the bad guy tank
*/
enum tankstate
{
    PATROL, // figure out how far the player is from the cpu, act based on that distance
    FIRE, // shoot of a round, then move
    RETREAT, // if it, or we just feel like it retreat
    TURN, // pick a direction to move in
    MOVE // move in a given direction
};

/*
    struct that stores the state of the current tank
*/
typedef struct 
{
    Vector2 tankpos; // 2D position of the badguy tank
    float tank_angle; // angle that the tank is faceing
    float commaned_angle; // angle that we commanded the tank to face
    enum tankstate currentstate;
    uint8_t agression; // how agressive the tank is
    int32_t hitpoints; // how many hits can the bullet take
    mqtthost_t *host; // pointer to this tank's host
    bool isAlive; // is this tank indeed alive
} badguytank_t;

/*
    header prototypes for the bad guy tank
*/

/*
    Helper function that returns a random angle between 0 and 2*pi
*/

float rnd_angle();

/*
    init the tank, start out each tank on patrol
    mqtthosts: array of mqtt hosts from the toml file
    badguys: array of badguy tanks
    ntanks: number of tanks that we want to create
    return 0 if everything word out, -1 if it didn't
*/
int init_tanks(mqtthost_t *mqtthosts,badguytank_t *badguys,uint32_t ntanks);

/*
    compute the next state of a tank and update the memory
    tank: a pointer to a tank object
*/

void tank_nextstate(badguytank_t *tank,gamestate_t *gamestate);

/*
    Make a random starting location for a bad guy tank
    This is kind of a place holder until I think of a better way to do this, but I want to code the tanks instead of messing with how the map works
*/
int place_random_tanks(badguytank_t *tankpos,uint32_t ntanks);

/*
    iterate through the array of bad guy tanks and draw them on screen
*/
int draw_badguy_tanks(Texture2D *tank_sp, badguytank_t *tanks,uint32_t ntanks);

/*
    Helper function to move a bad guy tank

*/

void move_badguy_tank(badguytank_t *tank,float dist);

/*
    function to register hits of badguy tanks
*/
void handle_bullet_hit(bullet_t *bullarr,uint32_t nbullet,badguytank_t *tankarr,uint32_t nbadguys);

#endif