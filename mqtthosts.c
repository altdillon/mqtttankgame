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
            printf("key: #%d: %s\n",i,key);
        }
    }

    toml_free(mqttbroker);
    return 0;
}