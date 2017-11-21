#ifndef MESSAGE_H
#define MESSAGE_H

#include <stdio.h>
#include <stdlib.h>
#include <my_global.h>
#include <mysql.h>

typedef struct
{
    int s_id;
    int r_id;
    char content[250];
    int timestamp;
    int state;
} Message;


/**
 * State of array
 * 0: normal
 * 1: can't connect yet
 * 2: no record
 */
typedef struct
{
    int state;
    int count;
    Message messages[1000];
} message_array;

int store_message   (char *send_name, char *receive_name, char* content,
                                                int timestamp, int state);
message_array get_history(char *name1, char *name2, int page);
message_array get_offline_messages();
int change_message_state(int message_id);

#endif
