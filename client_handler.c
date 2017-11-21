#include "client_handler.h"

char* not_found(char *rest){
    return rest;
}

void history(char *rest){

}

char *success_sent(char *rest){
    return rest;
}

char *user_exit(char *rest){
    return rest;
}

char *fail_login(char *rest){
    return rest;
}

user_array success_login(char *rest){
    char *token;
    user_array result;
    int count = 0;
    while ((token = strtok_r(rest, "|", &rest))){
        strcpy(result.users[count].name, strtok_r(token, " ", &token));
        result.users[count].state = atoi(token);
        count++;
    }
    result.count = count;
    return result;
}

char *fail_signin(char *rest){
    return rest;
}

user_array success_signin(char *rest){
    char *token;
    user_array result;
    int count = 0;
    while ((token = strtok_r(rest, "|", &rest))){
        strcpy(result.users[count].name, strtok_r(token, " ", &token));
        result.users[count].state = atoi(token);
        count++;
    }
    result.count = count;
    return result;
}

void change_user_state(user_array *array, char *name, int cur_state){
    int i;
    for(i = 0; i < array->count; i++){
        if(!strcmp(array->users[i].name, name)) {
            if(cur_state){
                array->users[i].state = 0;
            } else{
                array->users[i].state = 1;
            }
            return;
        }
    }
}

void add_user(user_array *array, char *name){
    strcpy(array->users[array->count].name, name);
    array->users[array->count].state = 1;
    array->count++;
}

void show_users(user_array array){
    int i;
    for ( i = 0;  i < array.count; i++ ) {
        printf("%s\t%d\n", array.users[i].name, array.users[i].state);
    }
}

char *already_login(char *rest){
    return rest;
}
