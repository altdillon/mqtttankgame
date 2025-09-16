#include <stdlib.h>
#include <time.h>
#include "map.h"


int random_walk(map_t *map,int width,int height,double walk_percent,int seed)
{
    // seed the random number generator
    if(seed == -1)
    {
        srand((unsigned)time(NULL));
    }
    else
    {
        srand((unsigned int)seed);
    }

    // init the map to all values of green gray scale
    for(uint8_t x=0;x<MAP_WIDTH;x++)
    {
        for(uint8_t y=0;y<MAP_HEIGHT;y++)
        {
            uint8_t green_scale = (rand() % 256) + 1;
            RGB_t block = {0x00,0x00,green_scale};
            map->map[x][y] = block;
        }
    }


    return 0;
}