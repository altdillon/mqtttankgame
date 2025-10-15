#include <stdlib.h>
#include <time.h>
#include "raylib.h"
#include "badguytank.h"
#include "map.h"

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
    // 

    return 0;
}