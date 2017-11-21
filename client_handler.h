#ifndef C_HANDLER_H
#define C_HANDLER_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define LOGIN_FAIL "430"
#define LOGIN_BEFORE "440"
#define LOGIN_SUCCESS "230"
#define SIGNUP_FAIL "451"
#define SIGNUP_SUCCESS "232"
#define EXIT "231"
#define SENT_SUCCESS "201"
#define HISTORY "350"
#define NOT_FOUND "404"
#define RECEIVE_MESSAGE "731"

#define SEND "SEND"
#define RECEIVE "RECV"

typedef struct {
    char name[30];
    int state;
} user;

typedef struct {
    int count;
    user users[100];
} user_array;

typedef struct {
    char sender[30];
    char content[250];
    char sent_time[30];
} message;

typedef struct {
    char name[30];
    int count;
    message messages[100];
} chat_history;

char *not_found(char *rest);
char *success_sent(char *rest);
char *user_exit(char *rest);
char *fail_login(char *rest);
user_array success_login(char *rest);
char *fail_signin(char *rest);
user_array success_signin(char *rest);
char *already_login(char *rest);

void add_user(user_array *array, char *name);
void change_user_state(user_array *array, char *name, int cur_state);
void show_users(user_array array);
void show_history(chat_history history);
#endif
