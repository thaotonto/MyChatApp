#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>
#include <setjmp.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/time.h>

#include "user.h"
#include "controller.h"
#include "message.h"
#include "listUserOnline.h"

#define BACKLOG 100 /* Number of allowed connections */
#define BUFF_SIZE 2098
#define DEFAULT_PORT 3000
#define TRUE 1
#define FALSE 0

void validArguments(int argc, char *argv[], int *port)
{
	if (argc > 1)
	{
		// Check valid port
		int i;
		char *port_str = argv[1];
		for (i = 0; port_str[i] != '\0'; i++)
		{
			if (!isdigit(port_str[i]))
			{
				printf("Port is invalid, using default port 3000\n");
				*port = DEFAULT_PORT;
				return;
			}
		}
		if (port_str[i] == '\0')
			*port = atoi(port_str);
	}
	else
	{
		*port = DEFAULT_PORT;
		return;
	}
}

extern node *head;

int main(int argc, char *argv[])
{
	int port = 0, rc, on = 1, nfds = 1, current_size = 0, i, j, k, desc_ready, end_server = FALSE, compress_array = FALSE;
	pid_t pid;
	int listen_sock, close_conn, new_sd = -1; /* file descriptors */
	char recv_data[BUFF_SIZE];
	int bytes_sent, bytes_received;
	struct sockaddr_in server;  /* server's address information */
	struct sockaddr_in *client; /* client's address information */
	int sin_size;
	struct pollfd fds[200];
	int timeout = 1, len;
	node *ptr;
	char message[2098], anouncer[2098];
	char *rest, content[250], sent_time[30];
	message_array history_message;
	validArguments(argc, argv, &port);

	// Create a socket
	if ((listen_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{ /* calls socket() */
		perror("\nError: ");
		return 0;
	}

	rc = setsockopt(listen_sock, SOL_SOCKET, SO_REUSEADDR,
					(char *)&on, sizeof(on));
	if (rc < 0)
	{
		perror("setsockopt() failed");
		close(listen_sock);
		exit(-1);
	}

	rc = ioctl(listen_sock, FIONBIO, (char *)&on);
	if (rc < 0)
	{
		perror("ioctl() failed");
		close(listen_sock);
		exit(-1);
	}

	// Setup Address structure
	bzero(&server, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = htonl(INADDR_ANY); /* INADDR_ANY puts your IP address automatically */

	// Bind the socket to the port
	if (bind(listen_sock, (struct sockaddr *)&server, sizeof(server)) == -1)
	{
		perror("\nError: ");
		return 0;
	}

	// Listen to the socket
	if (listen(listen_sock, BACKLOG) == -1)
	{ /* calls listen() */
		perror("\nError: ");
		return 0;
	}

	memset(fds, 0, sizeof(fds));
	fds[0].fd = listen_sock;
	fds[0].events = POLLIN;

	printf("<[SERVER STARTED]>\n");

	char output[BUFF_SIZE];
	int bytes_output;

	while (1)
	{
		rc = poll(fds, nfds, timeout);
		if (rc < 0)
		{
			perror("  poll() failed");
			break;
		}

		current_size = nfds;
		for (i = 0; i < current_size; i++)
		{
			/*********************************************************/
			/* Loop through to find the descriptors that returned    */
			/* POLLIN and determine whether it's the listening       */
			/* or the active connection.                             */
			/*********************************************************/
			if (fds[i].revents == 0)
				continue;

			/*********************************************************/
			/* If revents is not POLLIN, it's an unexpected result,  */
			/* log and end the server.                               */
			/*********************************************************/
			if (fds[i].revents != POLLIN)
			{
				printf("  Error! revents = %d\n", fds[i].revents);
				end_server = TRUE;
				break;
			}
			if (fds[i].fd == listen_sock)
			{
				/*******************************************************/
				/* Listening descriptor is readable.                   */
				/*******************************************************/
				printf("Listening socket is readable\n");

				/*******************************************************/
				/* Accept all incoming connections that are            */
				/* queued up on the listening socket before we         */
				/* loop back and call poll again.                      */
				/*******************************************************/
				do
				{
					/*****************************************************/
					/* Accept each incoming connection. If               */
					/* accept fails with EWOULDBLOCK, then we            */
					/* have accepted all of them. Any other              */
					/* failure on accept will cause us to end the        */
					/* server.                                           */
					/*****************************************************/
					new_sd = accept(listen_sock, NULL, NULL);
					if (new_sd < 0)
					{
						if (errno != EWOULDBLOCK)
						{
							perror("  accept() failed");
							end_server = TRUE;
						}
						break;
					}

					/*****************************************************/
					/* Add the new incoming connection to the            */
					/* pollfd structure                                  */
					/*****************************************************/
					printf(" New incoming connection - %d\n", new_sd);
					fds[nfds].fd = new_sd;
					fds[nfds].events = POLLIN;
					nfds++;

					/*****************************************************/
					/* Loop back up and accept another incoming          */
					/* connection                                        */
					/*****************************************************/
				} while (new_sd != -1);
			}

			/*********************************************************/
			/* This is not the listening socket, therefore an        */
			/* existing connection must be readable                  */
			/*********************************************************/

			else
			{
				printf("Descriptor %d is readable\n", fds[i].fd);
				close_conn = FALSE;
				/*******************************************************/
				/* Receive all incoming data on this socket            */
				/* before we loop back and call poll again.            */
				/*******************************************************/

				do
				{
					/*****************************************************/
					/* Receive data on this connection until the         */
					/* recv fails with EWOULDBLOCK. If any other         */
					/* failure occurs, we will close the                 */
					/* connection.                                       */
					/*****************************************************/
					bytes_received = recv(fds[i].fd, recv_data, BUFF_SIZE - 1, 0); //blocking
					if (bytes_received <= 0)
					{	
						perror("  recv() failed");
						close_conn = TRUE;
						strcpy(anouncer, deleteNodeWithValue(fds[i].fd));
						displayForward();
						if (!strcmp(anouncer, "UNAUTHORIZED")){
						} else {
							change_user_state(anouncer, 1);
							strcpy(message, "");
							strcpy(message, "231");
							strcat(message, "|");
							strcat(message, anouncer);
							ptr = head;
							while (ptr != NULL)
							{
								send(ptr->data, message, strlen(message), 0);
								ptr = ptr->next;
							}
						}
						break;
					}
					/*****************************************************/
					/* Check to see if the connection has been           */
					/* closed by the client                              */
					/*****************************************************/
					if (bytes_received == 1)
					{
						printf("  Connection closed\n");
						close_conn = TRUE;
						break;
					}

					/*****************************************************/
					/* Data was received                                 */
					/*****************************************************/
					len = bytes_received;
					printf("  %d bytes received\n", len);
					recv_data[bytes_received] = '\0';
					printf("Receive: |%s|\n\n", recv_data);
					Output *op = processCmd(recv_data);
					if (!strcmp(op->code, LOGIN_SUCCESS))
					{	
						strcpy(message, "");
						strcpy(message, op->code);
						strcat(message, "|");
						strcat(message, op->out1);
						ptr = head;
						while (ptr != NULL)
						{	
							send(ptr->data, message, strlen(message), 0);
							ptr = ptr->next;
						} 
						insertLast(op->out1, fds[i].fd);
						displayForward();
					}
					else
					{
						if (!strcmp(op->code, SIGNUP_SUCCESS))
						{	
							strcpy(message, "");
							strcpy(message, op->code);
							strcat(message, "|");
							strcat(message, op->out1);
							ptr = head;
							while (ptr != NULL)
							{
								send(ptr->data, message, strlen(message), 0);
								ptr = ptr->next;
							}
							insertLast(op->out1, fds[i].fd);
							displayForward();
						}
						else
						{
							if (!strcmp(op->code, EXIT))
							{
								deleteNodeWithKey(op->out1);
								displayForward();
								strcpy(message, "");
								strcpy(message, op->code);
								strcat(message, "|");
								strcat(message, op->out1);
								ptr = head;
								while (ptr != NULL)
								{
									send(ptr->data, message, strlen(message), 0);
									ptr = ptr->next;
								}
							}

							if(!strcmp(op->code, SENT_SUCCESS)) {
								ptr = head;
								while(ptr != NULL) {
									if (ptr->data == fds[i].fd) {
										strcpy(anouncer, ptr->key);
										break;
									}
									ptr = ptr->next;
								}
								rest = (char*)malloc(sizeof(op->out2) +1);
								strcpy(rest, op->out2);
								strcpy(sent_time,strtok_r(rest, "|", &rest));
								strcpy(content,strtok_r(rest, "|", &rest));
								store_message(anouncer, op->out1, content, sent_time, 0);
								strcpy(message, "");
								strcpy(message, "731");
								strcat(message, "|");
								strcat(message, anouncer);
								strcat(message, "|");
								strcat(message, op->out2);
								ptr = head;
								while (ptr != NULL)
								{	
									if (!strcmp(ptr->key, op->out1)) {
										change_message_state_on_sent(anouncer,op->out1,sent_time);
										send(ptr->data, message, strlen(message), 0);
										break;
									}
									ptr = ptr->next;
								}
							}


						}
					}

					if (!strcmp(op->code, HISTORY)) {
						ptr = head;
						while(ptr != NULL) {
						if (ptr->data == fds[i].fd) {
							strcpy(anouncer, ptr->key);
							break;
						}
						ptr = ptr->next;
						}
						history_message = get_history(anouncer, op->out1, atoi(op->out2));
						change_message_state(op->out1, anouncer);
						strcpy(message, "");
						strcpy(message, HISTORY);
						strcat(message, "|");
						strcat(message, op->out1);
						for(k = 0; k < history_message.count; k++) {
							strcat(message, "|");
							strcat(message, history_message.messages[k].send_name);
							strcat(message, "|");
							strcat(message, history_message.messages[k].sent_time);
							strcat(message, "|");
							strcat(message, history_message.messages[k].content);				
						}
						send(fds[i].fd, message, strlen(message), 0);
					} else {
						bytes_output = output_message(op, output);
						/*****************************************************/
						/* Echo the data back to the client                  */
						/*****************************************************/
						bytes_sent = send(fds[i].fd, output, bytes_output, 0); /* send to the client welcome message */
						if (bytes_sent <= 0)
						{
							perror("  send() failed");
							close_conn = TRUE;
							break;
						}
					}
					break;
				} while (TRUE);

				/*******************************************************/
				/* If the close_conn flag was turned on, we need       */
				/* to clean up this active connection. This            */
				/* clean up process includes removing the              */
				/* descriptor.                                         */
				/*******************************************************/
				if (close_conn)
				{	
					printf("close conn\n");
					close(fds[i].fd);
					fds[i].fd = -1;
					compress_array = TRUE;
				}

			} /* End of existing connection is readable             */
		}	 /* End of loop through pollable descriptors 
		/***********************************************************/
		/* If the compress_array flag was turned on, we need       */
		/* to squeeze together the array and decrement the number  */
		/* of file descriptors. We do not need to move back the    */
		/* events and revents fields because the events will always*/
		/* be POLLIN in this case, and revents is output.          */
		/***********************************************************/
		if (compress_array)
		{
			compress_array = FALSE;
			for (i = 0; i < nfds; i++)
			{
				if (fds[i].fd == -1)
				{
					for (j = i; j < nfds; j++)
					{
						fds[j].fd = fds[j + 1].fd;
					}
					i--;
					nfds--;
				}
			}
		}
	}
	while (end_server == FALSE)
		; /* End of serving running.    */

	/*************************************************************/
	/* Clean up all of the sockets that are open                 */
	/*************************************************************/
	for (i = 0; i < nfds; i++)
	{
		if (fds[i].fd >= 0)
			close(fds[i].fd);
	}
}