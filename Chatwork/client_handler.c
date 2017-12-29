#include "client_handler.h"

char *fail_signin(char *rest) {
    return rest;
}

char *not_found(char *rest) {
    return rest;
}

char *success_sent(char *rest) {
    return rest;
}

char *user_exit(char *rest) {
    return rest;
}

char *fail_login(char *rest) {
    return rest;
}

user_array success_login(char *rest) {
    char *token;
    user_array result;
    int count = 0;
    while ((token = strtok_r(rest, "|", &rest))) {
        strcpy(result.users[count].name, strtok_r(token, " ", &token));
        result.users[count].state = atoi(strtok_r(token, " ", &token));
        result.users[count].mess_count = atoi(token);
        count++;
    }
    result.count = count;
    return result;
}

user_array success_signin(char *rest) {
    char *token;
    user_array result;
    int count = 0;
    while ((token = strtok_r(rest, "|", &rest))) {
        strcpy(result.users[count].name, strtok_r(token, " ", &token));
        result.users[count].state = atoi(strtok_r(token, " ", &token));
        result.users[count].mess_count = atoi(token);
        count++;
    }
    result.count = count;
    return result;
}

void change_user_state(user_array *array, char *name, int cur_state) {
    int i;
    for (i = 0; i < array->count; i++) {
        if (!strcmp(array->users[i].name, name)) {
            if (cur_state) {
                array->users[i].state = 0;
            } else {
                array->users[i].state = 1;
            }
            return;
        }
    }
}

void add_user(user_array *array, char *name) {
    strcpy(array->users[array->count].name, name);
    array->users[array->count].state = 1;
    array->count++;
}

void show_users(user_array array) {
    int i;
    for (i = 0; i < array.count; i++) {
        printf("%s\t%d\t%d\n", array.users[i].name, array.users[i].state, array.users[i].mess_count);
    }
}

char *already_login(char *rest) {
    return rest;
}

void show_history(chat_history history) {
    printf("History with %s\n", history.name);
    int i;
    for (i = 0; i < history.count; i++) {
        printf("Sender: %s\nTime: %s\nContent: %s\n", history.messages[i].sender,
               history.messages[i].sent_time, history.messages[i].content);
    }
}

char *get_user_name(char *resource) {
    const char s[2] = " ";
    char *token;
    token = strtok(resource, s);
    return token;
}

char *get_user_name_and_state(char *resource) {
    int i;
    char *result;
    result = malloc(40);
    strcpy(result, resource);
    for (i = strlen(result) - 1; i >= 0; i--) {
        if(result[i] == ' '){
            result[i] = '\0';
            break;
        }
    }
    return result;
}


