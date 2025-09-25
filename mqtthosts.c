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

int load_toml_config(char *pathname,mqtthost_t *hosts)
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
                    
                    if(strcmp(device_member_key,"topic") == 0)
                    {
                    
                    }
                    else if(strcmp(device_member_key,"oncmd") == 0)
                    {

                    }
                    else if(strcmp(device_member_key,"offcmd") == 0)
                    {

                    }
                    else if(strcmp(device_member_key,"type") == 0)
                    {

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

    toml_free(mqttbroker);
    return 0;
}