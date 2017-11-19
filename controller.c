#include "controller.h"
#include <stdlib.h>
#include <string.h>
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

Output *processCmd (struct sockaddr_in *cliaddr, char *command_str) {
    struct Command_ *cmd = command(command_str);
    if (cmd == NULL) return;

    switch (cmd->code) {
    case USER:
        return processUSER(cliaddr, cmd->arg1, cmd->arg2);
    case SIGN:
        return processSIGN(cliaddr, cmd->arg1, cmd->arg2);
    case REQU:
        return processREQU(cliaddr, cmd->arg1, cmd->arg2);
    case SEND:
        return processSEND(cliaddr, cmd->arg1, cmd->arg2);
    case QUIT:
        return processQUIT(cliaddr, cmd->arg1, cmd->arg2);
    default:
        return NULL;
    }
}



Output *processUSER (struct sockaddr_in *cliaddr, char *name, char *pass) {
    user_array arr;
    int i;
    char buffer[2];
    Output *op = (Output *) malloc(sizeof(Output));
    if(check_user(name, pass) == 0){
        change_user_state(name, 0);
        strcpy(op->code, LOGIN_SUCCESS);
        strcpy(op->out1, "Wellcome ");
        strcat(op->out1, name);
        arr = get_users_list();
        if (arr.state == 0) {
            for (i = 0; i <  arr.count; i++) {
                strcat(op->out2, "|");
                strcat(op->out2, arr.users[i].name);
                strcat(op->out2, " ");
                sprintf(buffer,"%d",arr.users[i].state);
                strcat(op->out2, buffer);
            }
        }
    } else{
        strcpy(op->code, LOGIN_FAIL);
        strcpy(op->out1, "Invalid username or password");
    }
    return op;
}

Output *processSIGN (struct sockaddr_in *cliaddr, char *name, char *pass) {
    user_array arr;
    int i;
    char buffer[2]; 
    Output *op = (Output *) malloc(sizeof(Output));
    if(create_new_user(name, pass) == 0){
        change_user_state(name, 0);
        strcpy(op->code, LOGIN_SUCCESS);
        strcpy(op->out1, "Sign up success and login as ");
        strcat(op->out1, name);
        arr = get_users_list();
        if (arr.state == 0) {
            for (i = 0; i <  arr.count; i++) {
                strcat(op->out2, "|");
                strcat(op->out2, arr.users[i].name);
                strcat(op->out2, " ");
                sprintf(buffer,"%d",arr.users[i].state);
                strcat(op->out2, buffer);
            }
        }
    } else{
        strcpy(op->code, SIGNUP_FAIL);
        strcpy(op->out1, "Requested action aborted");
    }
    return op;
}

Output *processREQU (struct sockaddr_in *cliaddr, char *name, char *pass) {
    Output *op = (Output *) malloc(sizeof(Output));
    if(create_new_user(name, pass) == 0){
        strcpy(op->code, LOGIN_SUCCESS);
        strcpy(op->out1, "Sign up success and login as ");
        strcat(op->out1, name);
        //TODO: change status user
    } else{
        strcpy(op->code, SIGNUP_FAIL);
        strcpy(op->out1, "Requested action aborted");
    }
    return op;
}

Output *processSEND (struct sockaddr_in *cliaddr, char *name, char *pass) {
    Output *op = (Output *) malloc(sizeof(Output));
    if(create_new_user(name, pass) == 0){
        strcpy(op->code, LOGIN_SUCCESS);
        strcpy(op->out1, "Sign up success and login as ");
        strcat(op->out1, name);
        //TODO: change status user
    } else{
        strcpy(op->code, SIGNUP_FAIL);
        strcpy(op->out1, "Requested action aborted");
    }
    return op;
}

Output *processQUIT (struct sockaddr_in *cliaddr, char *name, char *pass) {
    Output *op = (Output *) malloc(sizeof(Output));
    if(create_new_user(name, pass) == 0){
        strcpy(op->code, LOGIN_SUCCESS);
        strcpy(op->out1, "Sign up success and login as ");
        strcat(op->out1, name);
        //TODO: change status user
    } else{
        strcpy(op->code, SIGNUP_FAIL);
        strcpy(op->out1, "Requested action aborted");
    }
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
    }

    if (!strcmp(code, "REQU")) {
        cmd->code = REQU;
    }

    if (!strcmp(code, "SEND")) {
        cmd->code = SEND;
    }

}
