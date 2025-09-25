#include "badguytank.h"

int init_tanks(mqtthost_t *mqtthosts,badguytank_h *badguys,uint32_t ntanks)
{
    for(uint32_t i=0;i<ntanks;i++)
    {
        badguys[i].currentstate = PATROL;
        
    }

    return 0;
}