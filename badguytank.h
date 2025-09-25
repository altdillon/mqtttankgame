#ifndef BADGUY_H
#define BADGUY_H

/*
    enum for the operating state of the bad guy tank
*/
enum tankstate
{
    PATROL; // figure out how far the player is from the cpu, act based on that distance
    SEARCH; // move the tank to face the player 
    FIRE; // shoot of a round, then move
    RETREAT; // if it, or we just feel like it retreat
    MOVE; // pick a direction to move in
};

/*
    header files for the bad guy tank
*/

#endif