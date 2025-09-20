#ifndef CONFIGFILE_H
#define CONFIGFILE_H

/*
    struct for storing enteries of entires
*/

typedef struct
{
    unsigned char *label; // label from the file
    unsigned char *topic; // mqttopic
    unsigned char *oncmd; // on command
    unsigned char *offcmd; // off command
}mqtthost_t;


#endif