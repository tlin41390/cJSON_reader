#include <stdio.h> 
#include <string.h> 
#include <sys/types.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include<sys/stat.h>  //stat
#include "cJSON.h" 
#define   FILENAME  "./config.json"

typedef struct 
{  
     char address[32];  
     int source_port;  
     int destination_port;  
     int payload;  
     int packets;   
}configurations;  

configurations cJSON_to_struct(char* text, configurations settings){
    cJSON *json,*item;
    int i =0;
    json = cJSON_Parse(text);

    item = cJSON_GetObjectItemCaseSensitive(json,"server_address");
    memcpy(settings.address,item->valuestring,strlen(item->valuestring));

    item = cJSON_GetObjectItemCaseSensitive(json,"source_port");
    settings.source_port = item->valueint;

    item= cJSON_GetObjectItemCaseSensitive(json,"destination_port");
    settings.destination_port = item->valueint;

    item = cJSON_GetObjectItemCaseSensitive(json,"size_of_payload");
    settings.payload = item->valueint;

    item = cJSON_GetObjectItemCaseSensitive(json,"number_of_udp_packets");
    settings.packets = item->valueint;

    cJSON_Delete(json);
    return settings;

}
size_t get_file_size(const char *filepath){
    if (filepath == NULL){
        return 0;
    }
    struct stat filestat;
    memset(&filestat,0,sizeof(struct stat));

    if(stat(filepath,&filestat)==0){
        return filestat.st_size;
    }else{
        return 0;
    }
}

configurations read_file(const char *filename)
{
    FILE *fp;

    configurations settings;

    size_t size = get_file_size(filename);
    if(size ==0){
        printf("size failed\n");
    }

    char *buffer = malloc(size+1);
    if(buffer == NULL){
        printf("malloc not successful\n");
    }
    memset(buffer,0,size+1);

    fp = fopen(filename,"rb");
    
    fread(buffer,1,size,fp);
    fclose(fp);

    settings = cJSON_to_struct(buffer,settings);
    free(buffer);
    return settings;
}

int main(int argc, char **argv){
    configurations balls = read_file(FILENAME);
    printf("%d\n",balls.packets);

    return 0;
}


