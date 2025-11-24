#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "raylib.h"
#include "raymath.h"
#include "badguytank.h"
#include "map.h"

float rnd_angle()
{
    srand((unsigned)time(NULL));
    // generate some balue between 0 and 1 and then multiply it by 2*pi
    float r = (float)rand() / RAND_MAX; 
    float angle = r * 2.0f * PI;
    return angle;
}

int init_tanks(mqtthost_t *mqtthosts,badguytank_t *badguys,uint32_t ntanks)
{
    for(uint32_t i=0;i<ntanks;i++)
    {
        badguys[i].currentstate = PATROL;
        // roll the dice between 1 and 20 to figure out how agressive this tank is
        uint8_t roll = rand() % (20 - 1 + 1) + 1;
        badguys[i].agression = roll;
        badguys[i].host = &mqtthosts[i];
        badguys[i].isAlive = true; // when the game starts we assume the tank to be alive
        // find a random angle to put the tank in 
        float r = (float)rand() / RAND_MAX; // some value between 0 and 1
        float angle = r * 2.0f * PI; // scale by 2PI, using raylib's pi constant
        badguys[i].tank_angle = angle;
        badguys[i].hitpoints = MAXTANKHEALTH;
    }

    return 0;
}

int place_random_tanks(badguytank_t *tankpos,uint32_t ntanks)
{
    // new cordnets for the tanks
    float tx = 0.0f;
    float ty = 0.0f;
    // get a number of tanks
    Vector2 tvects[ntanks];
    generate_vectors(tvects,ntanks);

    // iterate through the tanks and apply the vectors to the t tanks
    for(uint32_t i=0;i<ntanks;i++)
    {
        tankpos[i].tankpos = tvects[i]; // pretty much just copy it over
    }

    return 0;
}

int draw_badguy_tanks(Texture2D *tank_sp, badguytank_t *tanks,uint32_t ntanks)
{
    // width and height of the tank
    int frame_width = tank_sp->width;
    int frame_height = tank_sp->height;
    Vector2 origin = {(float)frame_width,(float)frame_height};
    Rectangle srcRec = {0.0f,0.0f,(float)frame_width,(float)frame_height}; // src rectangle
    Rectangle dstRec = {0.0f,0.0f,frame_width*2.0f,frame_height*2.0f}; // destonation where we're actually drawing this on the screen, will be defined in the for loop
    // interate through all the bad buy tanks and draw them to the screen
    for(uint32_t i=0;i<ntanks;i++) 
    {
        if(tanks[i].isAlive) // is the tank alive?
        {
            Vector2 tpos = tanks[i].tankpos;
            // update the destination rectangle
            dstRec.x = tpos.x;
            dstRec.y = tpos.y;
            float tangle = tanks[i].tank_angle * (180/PI);
            DrawTexturePro(*tank_sp,srcRec,dstRec,origin,tangle,WHITE);
        }
    }

    return 0;
}

void tank_nextstate(badguytank_t *tank, gamestate_t *gamestate)
{
    // general things about the game state to compute before we start doing anything for the state
    // compute distance
    float pdist = Vector2Distance(tank->tankpos, gamestate->player->spritePos);

    // const for dx and dy
    float tank_dx = 1.0f;
    float tank_dy = 1.0f;

    enum tankstate next_state = tank->currentstate;

    switch (tank->currentstate)
    {
        case PATROL:
        {
            // vector from tank to player
            Vector2 distVect = Vector2Subtract(gamestate->player->spritePos, tank->tankpos);
            // compute angle of that vector
            float pangle = Vector2Angle((Vector2){1, 0}, distVect) + (PI/2);
            //tank->tank_angle = pangle;
            if(pdist < 480.0f)
            {
                next_state = TURN;
            }
            else 
            {
                next_state = PATROL;
            }

            if(pdist < 400.0f)
            {
                next_state = MOVE;
            }
            tank->commaned_angle = pangle; 
            break;
        }

        case TURN:
        {
            tank->tank_angle = tank->commaned_angle;
            next_state = PATROL;
            break;
        }

        case MOVE:
        {
            float movev = 8.0f;
            move_badguy_tank(tank,movev);
            if(pdist > 100.0f)
            {
                next_state = PATROL;
            }
            else
            {
                next_state = FIRE;
            }


            break;
        }

        case FIRE:
        {
            // shoot at the planer some how
            next_state = PATROL;
            break;
        }
    }

    tank->currentstate = next_state;
}


void move_badguy_tank(badguytank_t *tank,float dist)
{
    float angle = tank->tank_angle + (PI/2) + PI;
    // compute the delta for x and y
    float deltaX = cosf(angle) * dist;
    float deltaY = sinf(angle) * dist;
    // wrap it up as a raylib vector
    Vector2 ds = {deltaX,deltaY};
    tank->tankpos = Vector2Add(tank->tankpos,ds);
}


void handle_bullet_hit(bullet_t *bullarr,uint32_t nbullet,badguytank_t *tankarr,uint32_t nbadguys)
{
    const float hitrad = 20.0f;
    // loop through all the bullets and bad guy tanks and figure out if they're within hitrad units of each other
    for(uint32_t i=0;i<nbullet;i++)
    {
        for(uint32_t j=0;j<nbadguys;j++)
        {
            // for the sake of readablity pull out the structs for the bullet and the bad guy tank
            badguytank_t badguy = tankarr[j];
            bullet_t thebullet = bullarr[i];
            // subtract the position vectors from eachother
            Vector2 vdist = Vector2Subtract(badguy.tankpos,thebullet.bullet_pos);
            float dist = Vector2Length(vdist);
            if(dist < hitrad)
            {
                //printf("%s\n","HIT!!!");
                tankarr[j].hitpoints -= bullarr[i].damage; // subtract the damage 
                if(tankarr[j].hitpoints < 0) // if this is less than zero then then the bad guy tank is dead
                {
                    tankarr[j].isAlive = false;
                }
            }
        }
    }
}