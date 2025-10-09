#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "raylib.h"
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
    for(uint32_t x=0;x<MAP_WIDTH;x++)
    {
        for(uint32_t y=0;y<MAP_HEIGHT;y++)
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


int draw_map(map_t *gmap,int startX,int startY,int width,int height)
{
    const uint32_t sideX = 8;
    const uint32_t sideY = 8;
    uint32_t cordX = 0;
    uint32_t cordY = 0;
    for(uint32_t x=0;x<width;x++)
    {
        for(uint32_t y=0;y<height;y++)
        {
            // get the RGB_t
            RGB_t mcolor = gmap->map[x][y];
            //uint8_t alpha = (rand() % 256) + 1; // random alpha/transpericy value because it's cool
            uint8_t alpha = 128; 
            Color rcolor = (Color) {mcolor.R,mcolor.G,mcolor.B,alpha};
            // figure out where we're going to draw it
            cordX = x * sideX + startX;
            cordY = y * sideY + startY;
            DrawRectangle(cordX,cordY,sideX,sideY,rcolor);
        }
    }
}


Vector2 find_startsplot(map_t *gamemap)
{
    const uint32_t clearning_threshhold = 3;
    const uint32_t dstep = 16; // how big of steps we're taking through the world to find a thresh hold
    uint32_t map_stepX = MAP_WIDTH / dstep;
    uint32_t map_stepY = MAP_HEIGHT / dstep;
    Vector2 retplace = {0.0f,0.0f}; // location where we're going to put a value
    const uint32_t nplaces = 10; // places where we can potintally find a value
    uint32_t clearning_count = 0;
    Vector2 clearings[nplaces]; 
    // iterate through everyone and find a value
    for(uint32_t x=0;x<map_stepX;x+=dstep)
    {
        for(uint32_t y=0;y<map_stepY;y+=dstep)
        {
            //uint32_t sumpx = gamemap->map[x][y].R + gamemap->map[x][y].G + gamemap->map[x][y].B;
            uint32_t sumpx = 0;
            // go through all the pixles in the dstep * dstep square and figure out how populated this space is
            for(uint32_t i=x;i<(x+dstep);i++)
            {
                for(uint32_t j=y;j<(y+dstep);j++)
                {
                    sumpx += gamemap->map[i][j].R + gamemap->map[i][j].G + gamemap->map[i][j].B;
                }
            }
            // figure out if it's a clearing that we can use
            if(sumpx < clearning_threshhold)
            {
                float vx = (float)(x + dstep/2);
                float vy = (float)(y + dstep/2);
                clearings[clearning_count] = (Vector2) {vx,vy};
                clearning_count ++;
                if(clearning_count > nplaces)
                {
                    break; // we're out of places to keep that clearing
                }
            }
        }
    }

    // use a random number to select one of the clearings
    uint32_t iclear = (rand() % nplaces) + 1;
    retplace = clearings[iclear];

    return retplace;
}