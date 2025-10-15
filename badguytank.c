#include <stdlib.h>
#include <time.h>
#include "badguytank.h"
#include "map.h"

int init_tanks(mqtthost_t *mqtthosts,badguytank_h *badguys,uint32_t ntanks)
{
    for(uint32_t i=0;i<ntanks;i++)
    {
        badguys[i].currentstate = PATROL;
        // roll the dice between 1 and 20 to figure out how agressive this tank is
        uint8_t roll = rand() % (20 - 1 + 1) + 1;
        badguys[i].agression = roll;
        badguys[i].host = &mqtthosts[i];
    }

    return 0;
}

int place_random_tanks(Vector2 *tankpos,uint32_t ntanks)
{
    // new cordnets for the tanks
    float tx = 0.0f;
    float ty = 0.0f;
    // get a number of tanks
    Vector2 tvects[MAXBADGUYTANKS];

    for(uint32_t i=0;i<ntanks;i++)
    {
        
    }

    return 0;
}