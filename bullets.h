#ifndef BULLETS_H
#define BULLETS_H
#include <stdint.h>
#include "raylib.h"
#define MAX_BULLETS 100 // number of rounds that can be on the screen

/*
    Struct for storing the state of a bullet
*/

typedef struct
{
    Vector2 org_pos; // starting position of the bullet
    Vector2 bullet_pos; // current position of the bullet
    Vector2 bullet_ds; // velocity of the bullet
    uint32_t id; // current ID of the bullet
    float bulletV; // velocity of the bullet
} bullet_t;

/*
    Launch a bullet from the tank
    nbullets: current number of bullets on the screen
    bulletarr: array of bullets
    start_pos: starting position of the bullet
    start_dir: starting direction of the bullet, bullet will fallow this vector
*/
int launch_bullet(uint32_t nbullets,bullet_t *bulletarr,Vector2 start_pos,Vector2 start_dir);

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

#endif