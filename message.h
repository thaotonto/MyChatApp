#ifndef MESSAGE_H
#define MESSAGE_H

#include <stdio.h>
#include <stdlib.h>
#include <my_global.h>
#include <mysql.h>

typedef struct
{
    int id;
    char send_name[30];
    char receive_name[30];
    char content[250];
    char sent_time[30];
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
                                                char* sent_time, int state);
message_array get_history(char *name1, char *name2, int page);
int get_offline_messages(char *sendName, char *receiveName);
int change_message_state(char *sendName, char *receiveName);
int change_message_state_on_sent(char *sendName, char *receiveName, char* sent_time);

#endif
