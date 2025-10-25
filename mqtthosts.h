#ifndef CONFIGFILE_H
#define CONFIGFILE_H
#define MAXHOSTS 20
#define MAXSTRLEN 80
#include <stdint.h>
#include <MQTTClient.h> // paho mqtt

/*
    Struct for the mqtt host
*/
typedef struct
{
    unsigned char hostip[MAXSTRLEN];
    uint64_t hostport;
} brocker_t;

/*
    Struct for all the states assoated with a phao mqtt lib
    "keep yo stinkn' objects out of my C code"
*/

typedef struct 
{
    MQTTClient client; // client object
    MQTTClient_connectOptions conn_opts; // connection options
    MQTTClient_message pubmsg; // publication messesage
    MQTTClient_deliveryToken token;
    int rc;
} pahostate_t;

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

int load_toml_config(char *pathname,mqtthost_t *hosts,brocker_t *brocker);

/*
    init the mqtt object
    https://eclipse.dev/paho/files/mqttdoc/MQTTClient/html/pubasync.html
*/

int init_mqtt(pahostate_t *pstate,unsigned char *brockeraddr);

// TODO: write a publish function 
int async_publish(brocker_t *brocker,unsigned char *topic,unsigned char *message);

#endif