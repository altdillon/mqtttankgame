#ifndef CONFIGFILE_H
#define CONFIGFILE_H
#define MAXHOSTS 20

/*
    struct for storing enteries of entires
    brocker ip and port information will be stored for every entery.  It just makes implentation easier, that's all...
*/

typedef struct
{
    unsigned char *label; // label from the file
    unsigned char *topic; // mqttopic
    unsigned char *oncmd; // on command
    unsigned char *offcmd; // off command
    unsigned char *broker_ip; // ip address of the broker as a string
    unsgiend char *host_port; // port of the broker as a string
}mqtthost_t;

/*
    Load config file
    pathname: file path of the file that we want to load
    mqtt_hosts: array of points
    return the number of hots loaded, or -1 if error
*/

int load_toml_config(char *pathname,mqtthost_t *hosts);

#endif