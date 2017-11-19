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
#include <sys/wait.h> /*****  For waitpid.                   *****/
#include <setjmp.h>   /*****  For sigsetjmp and siglongjmp.  *****/

#include "user.h"
#include "controller.h"

// #define USER_FILE "account.txt"
#define BACKLOG 100 /* Number of allowed connections */
#define BUFF_SIZE 2048
#define DEFAULT_PORT 3000

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
		printf("(ERROR) To few arguments!\n");
		exit(EXIT_FAILURE);
	}
}

void sig_chld(int signo)
{
	pid_t pid;
	int stat;
	while ((pid = waitpid(-1, &stat, WNOHANG)) > 0)
		printf("child %d terminated\n", pid);
	return;
	/* WNOHANG: waitpid does not block
    while loop: waitpid repeatedly until there is no child
    process change status, i.e until waitpid returns 0. */
}

int no_user = 0;
int no_session = 0;

int main(int argc, char *argv[])
{
	int port = 0;
	pid_t pid;
	int listen_sock, conn_sock; /* file descriptors */
	char recv_data[BUFF_SIZE];
	int bytes_sent, bytes_received;
	struct sockaddr_in server;  /* server's address information */
	struct sockaddr_in *client; /* client's address information */
	int sin_size;

	validArguments(argc, argv, &port);

	// Create a socket
	if ((listen_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{ /* calls socket() */
		perror("\nError: ");
		return 0;
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

	printf("<[SERVER STARTED]>\n");

	char output[BUFF_SIZE];
	int bytes_output;

	while (1)
	{
		//accept request
		sin_size = sizeof(struct sockaddr_in);
		client = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in));
		if ((conn_sock = accept(listen_sock, (struct sockaddr *)client, &sin_size)) == -1)
			perror("\nError: ");

		if (strcmp(inet_ntoa(client->sin_addr), "0.0.0.0") != 0)
		{
			if ((pid = fork()) == 0)
			{

				printf("You got a connection from %s\n", inet_ntoa(client->sin_addr)); /* prints client's IP */

				printf("%s:\n", "Child created for dealing with client requests");

				close(listen_sock); /* child closes listening socket */

				while (1)
				{
					//receives message from client
					bytes_received = recv(conn_sock, recv_data, BUFF_SIZE - 1, 0); //blocking
					if (bytes_received <= 0)
					{
						printf("\nConnection closed\n");
						break;
					}
					else
					{
						recv_data[bytes_received] = '\0';
						printf("\nReceive: |%s|\n", recv_data);

						Output *op = processCmd(client, recv_data);
						bytes_output = output_message(op, output);
					}

					//echo to client
					bytes_sent = send(conn_sock, output, bytes_output, 0); /* send to the client welcome message */
					if (bytes_sent <= 0)
					{
						printf("\nConnection closed\n");
						break;
					}
				} //end conversation
				  // Remove from session
				// removeSession(client);

				// Write list-user to file
				// writeUser(USER_FILE);
				close(conn_sock); /* done with this client */

				exit(0); /* child terminates */
			}
		}
		signal(SIGCHLD, sig_chld);
		close(conn_sock); /* parent closes connected socket */
	}

	close(listen_sock);
	return 0;
}
