#ifndef CONFIGFILE_H
#define CONFIGFILE_H
#define MAXHOSTS 20
#define MAXSTRLEN 80
#include <stdint.h>

/*
    struct for storing enteries of entires
    brocker ip and port information will be stored for every entery.  It just makes implentation easier, that's all...
*/

typedef struct
{
    unsigned char label[MAXSTRLEN]; // label from the file
    unsigned char topic[MAXSTRLEN]; // mqttopic
    unsigned char oncmd[MAXSTRLEN]; // on command
    unsigned char offcmd[MAXSTRLEN]; // off command
    unsigned char broker_ip[MAXSTRLEN]; // ip address of the broker as a string
    uint64_t host_port; // port of the broker as a string
}mqtthost_t;

/*
    Load config file
    pathname: file path of the file that we want to load
    mqtt_hosts: array of points
    return the number of hots loaded, or -1 if error
*/

int load_toml_config(char *pathname,mqtthost_t *hosts);

#endif