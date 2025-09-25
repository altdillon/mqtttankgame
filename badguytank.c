#include <stdlib.h>
#include <time.h>
#include "badguytank.h"

int init_tanks(mqtthost_t *mqtthosts,badguytank_h *badguys,uint32_t ntanks)
{
    for(uint32_t i=0;i<ntanks;i++)
    {
        badguys[i].currentstate = PATROL;
        // roll the dice between 1 and 20 to figure out how agressive this tank is
        uint8_t roll = rand() % (20 - 1 + 1) + 1;
        badguys[i].agression = roll;
    }

    return 0;
}