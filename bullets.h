#ifndef BULLETS_H
#define BULLETS_H
#include <stdint.h>
#include "raylib.h"
#define MAX_BULLETS 100 // number of rounds that can be on the screen
#define MAX_TICKS 120 // max number of frames that a bullet and stay alive for

/*
    Struct for storing the state of a bullet
*/

typedef struct
{
    Vector2 org_pos; // starting position of the bullet
    Vector2 bullet_pos; // current position of the bullet
    Vector2 bullet_ds; // velocity of the bullet
    uint32_t id; // current ID of the bullet
    uint32_t ticks; // how many frames has this bullet been alive
    uint32_t damage; // how much damage this bullet can do to a target
} bullet_t;

/*
    Launch a bullet from the tank
    nbullets: current number of bullets on the screen
    bulletarr: array of bullets
    start_pos: starting position of the bullet
    start_dir: starting direction of the bullet, bullet will fallow this vector
    speed: speed of bullet
*/
int launch_bullet(uint32_t nbullets,bullet_t *bulletarr,Vector2 start_pos,Vector2 start_dir,float speed);

/*
    Update the state of all the bullets
    bullarr: array of bullets on the screen
    nbullets: number of bullets on the screen
*/

void update_bullets(bullet_t *bullarr,uint32_t nbullets);

/*
    Draw bullets
*/
void draw_bullets(bullet_t *bullarr,uint32_t nbullets);

/*
    Helper function to remove bullets if they've been flying to long.
    Take in an array of bullets and the number of bullets.  Return the new number of bullets
*/
uint32_t enforce_bullet_range(bullet_t *bullarr,uint32_t nbullets);

#endif