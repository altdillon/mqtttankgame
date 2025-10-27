#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include "toml.h"
#include "mqtthosts.h"

/*
    error helper from the exsample code on github
*/
static void error(const char* msg, const char* msg1)
{
    fprintf(stderr, "ERROR: %s%s\n", msg, msg1?msg1:"");
    exit(1);
}

int load_toml_config(char *pathname,mqtthost_t *hosts,brocker_t *brocker)
{
    uint64_t host_port;
    char host_ip[80];

    FILE *fp;
    char errorbuf[200];
    // try to read and parse the toml file, return a -1 if it fails
    fp = fopen(pathname,"r");
    if(!fp)
    {
        error("cannot open toml file",strerror(errno));
        //fprintf(stderr,"TOML FILE ERROR: %s%s\n",strerror(errno));
        return -1;
    }

    // if we made it to the point where the file is open, lets read in a file
    toml_table_t *mqtthostdata = toml_parse_file(fp,errorbuf,sizeof(errorbuf));
    fclose(fp); // let the host os know that we're done with the file

    // was the file loaded?  if not return a -2 and print an error code
    if(!mqtthostdata)
    {
        error("cannot parse ",errorbuf);
    }

    // pull out the name of the server
    toml_table_t *mqttbroker = toml_table_table(mqtthostdata,"mqtthost");
    if(mqttbroker)
    {
        uint32_t len = toml_table_len(mqttbroker);
        for(uint32_t i=0;i<len;i++)
        {
            int keylen;
            const char *key = toml_table_key(mqttbroker,i,&keylen);
            //toml_value_t value = toml_table_string(mqttbroker,key);
            //printf("key: #%d: %s, data: %s\n",i,key,value.u.s);
            if(strcmp(key,"hostip") == 0)
            {
                toml_value_t value = toml_table_string(mqttbroker,key);
                strcpy(host_ip,value.u.s);
            }
            else if(strcmp(key,"hostport") == 0)
            {
                toml_value_t value = toml_table_int(mqttbroker,key);
                host_port = value.u.i;
            }
        }
    }

    uint32_t host_count = 0;
    // iterate through all of the devices and add them to the mqtthost_toml_table_lent array
    // see line ~30 of https://github.com/arp242/toml-c/blob/main/example/table.c
    toml_table_t *devices = toml_table_table(mqtthostdata,"devices");
    if(devices)
    {
        // iterate through the table keys and build up a list of members
        uint32_t table_length = toml_table_len(devices);
        for(uint32_t i=0;i<table_length;i++)
        {
            // pull out a key
            uint32_t key_length;
            const char *device_key = toml_table_key(devices,i,&key_length); 
            // pull out the commands for the mqtt thing
            //printf("%s\n",device_key);
            toml_table_t *adevice = toml_table_table(devices,device_key); // table for an indivdual device
            // pull down the elements for a devie
            if(adevice)
            {
                uint32_t device_table_length = toml_table_len(adevice);
                uint32_t member_key_length;
                for(uint32_t j=0;j<device_table_length;j++)
                {
                    const char *device_member_key = toml_table_key(adevice,j,&member_key_length);
                    // copy the mqtt broker and host port to the element 
                    //hosts[host_count].broker_ip = host_ip;
                    strcpy(hosts[host_count].broker_ip,host_ip);
                    hosts[host_count].host_port = host_port;
                    // figure out which elements we're writting to 
                    toml_value_t memberdata;
                    if(strcmp(device_member_key,"topic") == 0)
                    {
                        memberdata = toml_table_string(adevice,"topic");
                        strcpy(hosts[host_count].topic,memberdata.u.s);
                    }
                    else if(strcmp(device_member_key,"oncmd") == 0)
                    {
                        memberdata = toml_table_string(adevice,"oncmd");
                        strcpy(hosts[host_count].oncmd,memberdata.u.s);
                    }
                    else if(strcmp(device_member_key,"offcmd") == 0)
                    {
                        memberdata = toml_table_string(adevice,"offcmd");
                        strcpy(hosts[host_count].offcmd,memberdata.u.s);
                    }
                    else if(strcmp(device_member_key,"type") == 0)
                    {
                        memberdata = toml_table_string(adevice,"type");
                        strcpy(hosts[host_count].label,memberdata.u.s);
                    }
                }
            } 
            host_count ++; // incriment the host count 
        }

    }
    else
    {
        return -1; // just fail if we don't have anything
    }


    strcpy(brocker->hostip,host_ip);
    brocker->hostport = host_port;

    toml_free(mqttbroker);
    return host_count;
}

int init_mqtt(pahostate_t *pstate,unsigned char *brockeraddr)
{
    pstate->conn_opts = (MQTTClient_connectOptions)MQTTClient_connectOptions_initializer;
    pstate->pubmsg = (MQTTClient_message)MQTTClient_message_initializer;
    
    

    return 0;
}


volatile MQTTClient_deliveryToken deliveredtoken;
void delivered(void *context, MQTTClient_deliveryToken dt)
{
    printf("Message with token value %d delivery confirmed\n", dt);
    deliveredtoken = dt;
}
int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{
    int i;
    char* payloadptr;
    printf("Message arrived\n");
    printf("     topic: %s\n", topicName);
    printf("   message: ");
    payloadptr = message->payload;
    for(i=0; i<message->payloadlen; i++)
    {
        putchar(*payloadptr++);
    }
    putchar('\n');
    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return 1;
}
void connlost(void *context, char *cause)
{
    printf("\nConnection lost\n");
    printf("     cause: %s\n", cause);
}

int async_publish(brocker_t *brocker,unsigned char *topic,unsigned char *message)
{
    const unsigned char *client_id = "tankgame";
    const uint32_t qos = 1;
    // create some objects 
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;
    int rc;

    // build up the url or whatever idaf
    unsigned char brokerAddr[MAXSTRLEN];
    sprintf(brokerAddr,"tcp://%s:%d",brocker->hostip,brocker->hostport);

    
    MQTTClient_create(&client,brokerAddr,client_id,MQTTCLIENT_PERSISTENCE_NONE,NULL);
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;
    
    MQTTClient_setCallbacks(client,NULL,connlost,msgarrvd,delivered);

    if((rc = MQTTClient_connect(client,&conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to connect, return code: %d\n",rc);
        return -1; // return -1 as a fail
    }

    // set the string options for the payload
    //strcpy(pubmsg.payload,message);
    pubmsg.payload = message; 
    pubmsg.payloadlen = strlen(message);
    pubmsg.qos = qos;
    pubmsg.retained = 0;
    deliveredtoken = 0;
    // actually send the message
    MQTTClient_publishMessage(client,topic,&pubmsg,&token);
    // NOTE: possibly print a message about waiting to send a message
    while(deliveredtoken != token);
    // after the message is sent, do a clean up
    MQTTClient_disconnect(client,10000);
    MQTTClient_destroy(&client);

    return rc;
}

int find_mqtthost(char *hostname,mqtthost_t *host,mqtthost_t *hostarray,uint32_t nhosts)
{
    if(strlen(hostname) < 1)
    {
        for(uint32_t i=0;i<nhosts;i++)
        {

        }
    }

    return -1; // if we get there, just say we failed
}