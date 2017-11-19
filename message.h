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

int store_message   (int s_id, int r_id, char* content,
                                                int timestamp, int state);
message_array get_history(int id_1, int id_2);
message_array get_offline_messages();
int change_message_state(int s_id, int r_id, int timestamp);

#endif
