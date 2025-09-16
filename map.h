#ifndef MAP_H
#define MAP_H
#include <stdint.h>
#define MAP_HEIGHT 30
#define MAP_WIDTH 30

/*
    Structure for RGB values
*/
typedef struct 
{
    uint8_t R;
    uint8_t G;
    uint8_t B;
} RGB_t;

/*
    structure for holding the map.  The map will be defined as a 2D matrix or RGB values
*/
typedef struct 
{
    RGB_t map[MAP_HEIGHT][MAP_WIDTH];
} map_t;

/*
    Function for the random walk
*/
int random_walk(int width,int height,double walk_percent,int seed);

#endif