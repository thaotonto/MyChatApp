

#ifndef CONTROL_H
#define CONTROL_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#include "user.h"

#define LOGIN_FAIL "430"
#define LOGIN_SUCCESS "230"
#define SIGNUP_FAIL "451"

#define CMD_ARG_1_LEN 30
#define CMD_ARG_2_LEN 30
#define CMD_ARG_3_LEN 250
#define OUT_1_LEN 50
#define OUT_2_LEN 2048
#define CODE_LEN 3          // do dai lon nhat cua ma lenh

enum CommandCode {
    USER,
    SIGN,
    QUIT,
    REQU,
    SEND
};

struct Command_ {
    enum CommandCode code;
    char arg1[CMD_ARG_1_LEN + 1];
    char arg2[CMD_ARG_2_LEN + 1];
    char arg3[CMD_ARG_3_LEN + 1];
};

typedef struct Output_ {
    char code[CODE_LEN + 1];
    char out1[OUT_1_LEN + 1];
    char out2[OUT_1_LEN + 1];
} Output;

int output_message (Output *op, char *str);
Output *processCmd (struct sockaddr_in *cliaddr, char *command_str);
Output *processUSER (struct sockaddr_in *cliaddr, char *name, char *pass);
Output *processSIGN (struct sockaddr_in *cliaddr, char *name, char *pass);
Output *processREQU (struct sockaddr_in *cliaddr, char *name, char *pass);
Output *processSEND (struct sockaddr_in *cliaddr, char *name, char *pass);
Output *processQUIT (struct sockaddr_in *cliaddr, char *name, char *pass);
struct Command_ *command (char *input_str);

#endif
