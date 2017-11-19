#include "controller.h"
/**
 * [op2str  description]
 * @param  op       [description]
 * @param  str      [command string]
 * @param  str_size [size of the full buffer]
 * @return          [string length of the command]
 */
int output_message (Output *op, char *str) {
    bzero(str, 2048);

    if (op == NULL) {
        strcpy(str, "Error: Command not found!");
    } else {
        strcpy(str, op->code);
        strcat(str, " ");
        strcat(str, op->out1);
        if(strlen(op->out2) > 0){
            strcat(str, "");
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
    default:
        return NULL;
    }
}



Output *processUSER (struct sockaddr_in *cliaddr, char *name, char *pass) {
    Output *op = (Output *) malloc(sizeof(Output));
    if(check_user(name, pass) == 0){
        strcpy(op->code, LOGIN_SUCCESS);
        strcpy(op->out1, "Wellcome lil'bitch");
    } else{
        strcpy(op->code, LOGIN_FAIL);
        strcpy(op->out1, "Invalid username or password");
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

    sscanf(input_str, "%s %s %s %s", code, argv1, argv2, argv3);

    if (!strcmp(code, "USER")) {
        cmd->code = USER;
        strcpy(cmd->arg1, argv1);
        strcpy(cmd->arg2, argv2);
        return cmd;
    }
}
