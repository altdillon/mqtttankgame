#include <stdlib.h>
#include <time.h>
#include <math.h>
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
            RGB_t block = {0x00,green_scale,0x00};
            map->map[x][y] = block;
        }
    }

    // define an X and Y, start sort of in the center
    uint32_t mx = width / 2;
    uint32_t my = height / 2;
    map->map[mx][my] = (RGB_t){0x00,0x00,0x00};
    // define a target for the drunk to walk to
    uint32_t floor_target = floor(width * height * walk_percent);
    uint32_t floor_count = 1;
    // define deltas for directions
    int8_t deltas[4][2] = {
        { 0,  1},  // up
        { 0, -1},  // down
        { 1,  0},  // right
        {-1,  0}   // left
    };

    // do a munch of steps until floor count is over floor target
    while(floor_count < floor_target)
    {
        // pick a random direction
        uint16_t rdir = rand() % 4;
        int dx = deltas[rdir][0];
        int dy = deltas[rdir][1];
        // add dx and dy to x and y
        mx += dx;
        my += dy; 

        // do some bounds checking, thanks to chat gpt lol
        // do bounds checking here (keep x,y inside map)
        if (mx < 1) mx = 1;
        if (mx >= width - 1) mx = width - 2;
        if (my < 1) my = 1;
        if (my >= height - 1) my = height - 2;

        if(map->map[mx][my].G > 0)
        {
            // now zero out wherver mx and my seem to be
            map->map[mx][my] = (RGB_t) {0x00,0x00,0x00};
            floor_count ++;
        }

    }

    return 0;
}