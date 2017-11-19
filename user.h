#ifndef USER_H
#define USER_H

#include <stdio.h>
#include <stdlib.h>
#include <my_global.h>
#include <mysql.h>

typedef struct
{
    int id;
    char name[30];
    char password[30];
    int state;
} User;


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
    User users[100];
} user_array;

User *get_user_by_id(int id);
int create_new_user(char *name, char *password);
int check_user(char *name, char *password);
user_array get_users_list();


#endif
