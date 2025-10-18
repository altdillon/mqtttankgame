#include <stdlib.h>
#include <time.h>
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


void tank_nextate(badguytank_t *tank,gamestate_t *gamestate)
{
    const float dx = 1.0f;
    const float dy = 2.0f;
    enum tankstate next_state;
    switch(tank->currentstate)
    {
        case PATROL:
            tank->tankpos.x += dx;
            tank->tankpos.y += dy;
            next_state = SEARCH;
        break;
        case SEARCH:
            // find the distance between this tank and the player
            float vdist = Vector2Distance(tank->tankpos,gamestate->player->spritePos);
            if(vdist < 2.0f * tank->agression) 
            {
                // turn to face the player
                float newAngle = Vector2Angle(tank->tankpos,gamestate->player->spritePos);
                tank->commaned_angle = newAngle;
            }
            else
            {
                float newAngle = rnd_angle();
                tank->commaned_angle = newAngle;
            }
            next_state = TURN;
        break;
        case TURN:
            if(tank->commaned_angle != tank->tank_angle)
            {
                tank->tank_angle += 0.1;
                next_state = TURN;
            }
            else
            {
                next_state = PATROL;
            }
        break;
    }
    tank->currentstate = next_state;
}