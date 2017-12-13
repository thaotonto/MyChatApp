#include "controller.h"
#include <stdlib.h>
#include <string.h>

#include "listUserOnline.h"

extern node *head;

/**
 * [op2str  description]
 * @param  op       [description]
 * @param  str      [command string]
 * @param  str_size [size of the full buffer]
 * @return          [string length of the command]
 */
int output_message (Output *op, char *str) {
    bzero(str, 2098);
    if (op == NULL) {
        strcpy(str, "Error: Command not found!");
    } else {
        strcpy(str, op->code);
        strcat(str, "|");
        strcat(str, op->out1);
        if(strlen(op->out2) > 0){
            strcat(str, op->out2);
        }
        free(op);
    }
    return (strlen(str) + 1);
}

Output *processCmd (char *command_str) {
    struct Command_ *cmd = command(command_str);
    if (cmd == NULL) return processNOTFOUND();

    switch (cmd->code) {
    case USER:
        return processUSER(cmd->arg1, cmd->arg2);
    case SIGN:
        return processSIGN(cmd->arg1, cmd->arg2);
    case QUIT:
        return processQUIT(cmd->arg1);
    case REQU:
        return processREQU(cmd->arg1, cmd->arg2);
    case SEND:
        return processSEND(cmd->arg2, cmd->arg1, cmd->arg3);
    default:
        return processNOTFOUND();
    }
}

Output *processNOTFOUND () {
    Output *op = (Output *) malloc(sizeof(Output));
    strcpy(op->code, NOT_FOUND);
    strcpy(op->out1, "Command not found");
    strcpy(op->out2, "");
    return op;
}

Output *processUSER (char *name, char *pass) {
    user_array arr;
    int i;
    char buffer[2];
    Output *op = (Output *) malloc(sizeof(Output));
    int state = check_user(name, pass);

    if(state == 0){
        change_user_state(name, 0);
        strcpy(op->code, LOGIN_SUCCESS);
        strcpy(op->out1, name);
        arr = get_users_list();
        if (arr.state == 0) {
            for (i = 0; i <  arr.count; i++) {
                if (i == 0) {
                    strcpy(op->out2, "|");
                } else {
                    strcat(op->out2, "|");
                }
                strcat(op->out2, arr.users[i].name);
                strcat(op->out2, " ");
                sprintf(buffer,"%d",arr.users[i].state);
                strcat(op->out2, buffer);
                strcat(op->out2, " ");
                sprintf(buffer,"%d", get_offline_messages(arr.users[i].name, name));
                strcat(op->out2, buffer);
            }
        }
    } else{
        if (state == 3) {
            strcpy(op->code, LOGIN_BEFORE);
            strcpy(op->out1, "User has already logged in");
            strcpy(op->out2, "");
        } else {
            strcpy(op->code, LOGIN_FAIL);
            strcpy(op->out1, "Invalid username or password");
            strcpy(op->out2, "");
        }
    }
    return op;
}

Output *processSIGN (char *name, char *pass) {
    user_array arr;
    int i;
    char buffer[2]; 
    Output *op = (Output *) malloc(sizeof(Output));
    if(create_new_user(name, pass) == 0){
        change_user_state(name, 0);
        strcpy(op->code, SIGNUP_SUCCESS);
        strcpy(op->out1, name);
        arr = get_users_list();
        if (arr.state == 0) {
            for (i = 0; i <  arr.count; i++) {
                if (i == 0) {
                    strcpy(op->out2, "|");
                } else {
                    strcat(op->out2, "|");
                }
                strcat(op->out2, arr.users[i].name);
                strcat(op->out2, " ");
                sprintf(buffer,"%d",arr.users[i].state);
                strcat(op->out2, buffer);
                strcat(op->out2, " ");
                sprintf(buffer,"%d", get_offline_messages(arr.users[i].name, name));
                strcat(op->out2, buffer);
            }
        }
    } else{
        strcpy(op->code, SIGNUP_FAIL);
        strcpy(op->out1, "Requested action aborted");
        strcpy(op->out2, "");
    }
    return op;
}

Output *processREQU (char *name, char *page) {
    Output *op = (Output *) malloc(sizeof(Output));
    strcpy(op->code, HISTORY);
    strcpy(op->out1, name);
    strcpy(op->out2, page);
    return op;
}

Output *processSEND (char *name, char *sent_time, char* content) {
    Output *op = (Output *) malloc(sizeof(Output));
    strcpy(op->code, SENT_SUCCESS);
    strcpy(op->out1, name);
    strcpy(op->out2, "|");
    strcat(op->out2, sent_time);
    strcat(op->out2, "|");
    strcat(op->out2, content);
    return op;
}

Output *processQUIT (char *name) {
    Output *op = (Output *) malloc(sizeof(Output));
    change_user_state(name, 1);
    strcpy(op->code, EXIT);
    strcpy(op->out1, name);
    strcpy(op->out2, "");
    return op;
}


/**
 * [command  description]
 * @param  input_str [command in raw string]
 * @return           [command in struct]
 */
struct Command_ *command (char *input_str) {

    char code[CODE_LEN + 1];
    char argv1[CMD_ARG_1_LEN + 1];
    char argv2[CMD_ARG_2_LEN + 1];
    char argv3[CMD_ARG_3_LEN + 1];

    struct Command_ *cmd = (struct Command_ *) malloc (sizeof(struct Command_));

    char *rest = input_str;

    strcpy(code,strtok_r(rest, "|", &rest));
    if (!strcmp(code, "USER")) {
        cmd->code = USER;
        strcpy(argv1,strtok_r(rest, "|", &rest));
        strcpy(argv2,strtok_r(rest, "|", &rest));
        strcpy(cmd->arg1, argv1);
        strcpy(cmd->arg2, argv2);
        return cmd;
    }

    if (!strcmp(code, "SIGN")) {
        cmd->code = SIGN;
        strcpy(argv1,strtok_r(rest, "|", &rest));
        strcpy(argv2,strtok_r(rest, "|", &rest));
        strcpy(cmd->arg1, argv1);
        strcpy(cmd->arg2, argv2);
        return cmd;
    }

    if (!strcmp(code, "QUIT")) {
        cmd->code = QUIT;
        strcpy(argv1,strtok_r(rest, "|", &rest));
        strcpy(cmd->arg1, argv1);
        return cmd;
    }

    if (!strcmp(code, "REQU")) {
        cmd->code = REQU;
        strcpy(argv1,strtok_r(rest, "|", &rest));
        strcpy(argv2,strtok_r(rest, "|", &rest));
        strcpy(cmd->arg1, argv1);
        strcpy(cmd->arg2, argv2);
        return cmd;
    }

    if (!strcmp(code, "SEND")) {
        cmd->code = SEND;
        strcpy(argv1,strtok_r(rest, "|", &rest));
        strcpy(argv2,strtok_r(rest, "|", &rest));
        strcpy(argv3,strtok_r(rest, "|", &rest));
        strcpy(cmd->arg1, argv1);
        strcpy(cmd->arg2, argv2);
        strcpy(cmd->arg3, argv3);
        return cmd;
    }

    return NULL;
}
