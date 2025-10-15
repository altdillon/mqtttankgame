#ifndef MAP_H
#define MAP_H
#include <stdint.h>
#include "raylib.h"
#define MAP_HEIGHT 400
#define MAP_WIDTH 400

/*
    Structure for RGB values
*/
typedef struct 
{
    uint8_t R;
    uint8_t G;
    uint8_t B;
    uint8_t alpha;
} RGB_t;

/*
    structure for holding the map.  The map will be defined as a 2D matrix or RGB values
*/
typedef struct 
{
    RGB_t map[MAP_WIDTH][MAP_HEIGHT];
} map_t;

/*
    Function for the random walk
*/
int random_walk(map_t *map,int width,int height,double walk_percent,int seed);

/*
    Function to draw the map
*/

int draw_map(map_t *gmap,int startX,int startY,int width,int height);

/*
    function to find a black spot in the map
    gamemap: takes in an object for the gamemap
    returns a 2D vector that says where we can place stuff
    */

Vector2 find_startsplot(map_t *gamemap);

/*
    Generate n number of random vectors
*/
void generate_vectors(Vector2 *vecs,uint32_t nvects);

#endif