#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include "client_handler.h"
#define BUFF_SIZE 2098

// #define CODE_LEN 4          // do dai lon nhat cua ma lenh
#define RECV_LEN 4          // do dai code nhan tu server

int client_sock_fd;
char name[30];
user_array array;
int chat_count = 0;
chat_history histories[5];

void signio_handler(int signo)
{
  char buff[BUFF_SIZE];
  char *rest;
  char code[RECV_LEN];
  char sender[30];
  char sent_time[30];
  char content[250];
  int i;

  int n = recv(client_sock_fd, buff, sizeof buff, 0);
  if (n>0) // if SIGIO is generated by a data arrival
    {
        buff[n] = '\0';
        printf("Received from server: %s\n", buff);
        rest = buff;
        strcpy(code, strtok_r(rest, "|", &rest));
        if(!strcmp(code, HISTORY)) {
            strcpy(histories[chat_count].name, strtok_r(rest, "|", &rest));
            histories[chat_count].count = 0;
            while(strlen(rest) != 0){
                i = histories[chat_count].count;
                strcpy(histories[chat_count].messages[i].sender, strtok_r(rest, "|", &rest));
                strcpy(histories[chat_count].messages[i].sent_time, strtok_r(rest, "|", &rest));
                strcpy(histories[chat_count].messages[i].content, strtok_r(rest, "|", &rest));
                histories[chat_count].count++;
            }
            show_history(histories[chat_count]);
            chat_count++;
        }
        if(!strcmp(code, SENT_SUCCESS)){
            printf("%s\n", success_sent(rest));
        }
        if(!strcmp(code, RECEIVE_MESSAGE)){
            strcpy(sender, strtok_r(rest, "|", &rest));
            strcpy(sent_time, strtok_r(rest, "|", &rest));
            strcpy(content, strtok_r(rest, "|", &rest));
            printf("Got message: '%s' at %s from %s\n", content, sent_time, sender);
        }
        if(!strcmp(code, LOGIN_SUCCESS)){
            printf("LOGIN\n" );
            if(strlen(name) <= 0) { //------> chua dang nhap
                strcpy(name, strtok_r(rest, "|", &rest));
                array = success_login(rest);
                printf("Loged in as %s\n", name);
                show_users(array);
            } else{ //----------> da dang nhap
                change_user_state(&array, already_login(rest), 0);
                show_users(array);
            }
        }
        if(!strcmp(code, LOGIN_FAIL)) {
            printf("%s\n", fail_login(rest));
        }
        if (!strcmp(code, LOGIN_BEFORE)) {
            printf("%s\n", fail_login(rest));
        }
        if(!strcmp(code, SIGNUP_SUCCESS)){
            if(strlen(name) <= 0) { //------> chua dang nhap
                strcpy(name, strtok_r(rest, "|", &rest));
                array = success_signin(rest);
                printf("Loged in as %s\n", name);
                show_users(array);
            } else{ //----------> da dang nhap
                add_user(&array, already_login(rest));
                show_users(array);
            }
        }
        if(!strcmp(code, SIGNUP_FAIL)){
            printf("%s\n", fail_signin(rest));
        }
        if(!strcmp(code, EXIT)){
            char *logout_user = user_exit(rest);
            if(strcmp(name, logout_user)){
                change_user_state(&array, logout_user, 1);
                show_users(array);
            } else {
                exit(0);
            }
        }
        if(!strcmp(code, NOT_FOUND)){
            printf("%s\n", not_found(rest));
        }
    }
}

void valid_arguments (int argc, char *argv[], char *serv_ip, int *serv_port);
void cmd_SEND(char *buff);
char *get_time();

int main(int argc, char *argv[]) {
    int server_port = 0;
    char server_ip[16] = "";        // 16 = (max length of ipv4 string) + 1
    // Check arguments
    valid_arguments(argc, argv, server_ip, &server_port);

    char buff[BUFF_SIZE];
    struct sockaddr_in server_addr; /* server's address information */
    int msg_len, bytes_sent, bytes_received;

    client_sock_fd = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    server_addr.sin_addr.s_addr = inet_addr(server_ip);

    if(connect(client_sock_fd, (struct sockaddr*)&server_addr, sizeof(struct sockaddr)) < 0) {
        printf("\nError!Can not connect to sever! Client exit imediately!");
        return 0;
    }

    // Signal driven I/O mode and NONBlOCK mode so that recv will not block
    if(fcntl(client_sock_fd, F_SETFL, O_NONBLOCK|O_ASYNC))
      printf("Error in setting socket to async, nonblock mode");

    signal(SIGIO, signio_handler); // assign SIGIO to the handler

    //set this process to be the process owner for SIGIO signal
    if (fcntl(client_sock_fd, F_SETOWN, getpid()) <0)
      printf("Error in setting own to socket");

    while(1){
        //send message
        __fpurge(stdin);
        bzero(buff, BUFF_SIZE);
        fgets(buff, BUFF_SIZE, stdin);
        buff[strlen(buff) - 1] = '\0';
        cmd_SEND(buff);

        // // Check exit
        // if (wanna_exit(buff)) return;E

        // Xu ly neu la command LOUT
        msg_len = strlen(buff) + 1;

        bytes_sent = send(client_sock_fd, buff, msg_len, 0);
        if(bytes_sent <= 0){
            printf("\nConnection closed!\n");
            break;
        }
    }

    //Step 4: Close socket
    close(client_sock_fd);
    return 0;
}



// Check arguments is valid or not. If valid ip -> *serv_ip, port -> &serv_port
void valid_arguments (int argc, char *argv[], char *serv_ip, int *serv_port) {
    if (argc > 2) {
        // Check valid ip address
        struct sockaddr_in tmp_addr;
        if (inet_pton(AF_INET, argv[1], &(tmp_addr.sin_addr)) == 0) {
            printf("IP Address is invalid\n!");
            exit(EXIT_FAILURE);
        } else {
            strcpy(serv_ip, argv[1]);
        }

        // Check valid port
        int i;
        char *port_str = argv[2];
        for (i = 0; port_str[i] != '\0'; i++) {
            if (!isdigit(port_str[i])) {
                printf("Port is invalid\n!");
                exit(EXIT_FAILURE);
            }
        }
        if (port_str[i] == '\0') *serv_port = atoi(port_str);
    } else {
        printf("(ERROR) To few arguments!\n");
        exit(EXIT_FAILURE);
    }
}

char *get_time(){
    char *result;
    time_t rawtime;
    struct tm * timeinfo;

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    result = asctime(timeinfo);
    result[strlen(result) - 1] = '\0';
    return result;
}

void cmd_SEND(char *buff){
    char rest[5], temp[300];
    strncpy(rest, buff, 4);
    if(strcmp(rest, SEND) == 0){
        strncpy(temp, buff + 5, strlen(buff) -4);
        sprintf(buff, "%s|%s|%s", rest, get_time(), temp);
    }
}
