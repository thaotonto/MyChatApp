#include <string.h>
#include <stdio.h>

typedef struct {
    char name[30];
    int state;
} user;

typedef struct {
    int count;
    user users[100];
} user_array;

void delete_user(user_array *array, char *name){
    int i, j;
    for(i = 0; i < array->count; i++){
        if(!strcmp(array->users[i].name, name)) {
            for(j = i;  j < array->count - 1; j++) {
                strcpy(array->users[j].name, array->users[j+1].name);
                array->users[j].state = array->users[j+1].state;
            }
            array->count--;
            return;
        }
    }
}

void main()
{
    int i;
    user_array array;
    array.count = 4;
    strcpy(array.users[0].name, "a");
    array.users[0].state = 1;
    strcpy(array.users[1].name, "b");
    array.users[1].state = 2;
    strcpy(array.users[2].name, "c");
    array.users[2].state = 3;
    strcpy(array.users[3].name, "d");
    array.users[3].state = 4;
    delete_user(&array, "d");

    user_array test = array;
    delete_user(&test, "a");
    for(i = 0; i < test.count; i++){
        printf("%s %d\n", test.users[i].name, test.users[i].state);
    }
}

