#include <sys/socket.h> // socket
#include <netinet/in.h> // struct sockaddr_in
#include <arpa/inet.h>	// inet_pton
#include <stdarg.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>

#define SERVERPORT 8989
#define BUFSIZE 4096
#define SOCKETERROR (-1)
#define SERVER_BACKLOG 100

typedef struct sockaddr_in SA_IN;
typedef struct sockaddr SA;

void *handle_connection(int);
int check(int exp, const char *msg);
int accept_new_connection(int server_socket);
int setup_server(short port, int backlog);

int main()
{
	int server_socket = setup_server(SERVERPORT, SERVER_BACKLOG);

	while (true)
	{
		printf("Waiting for connection...\n");
		int client_socket = accept_new_connection(server_socket);

		handle_connection(client_socket);
	}
	return 0;
}

int setup_server(short port, int backlog)
{
	int server_socket, client_socket, addr_size;
	SA_IN server_addr;

	check((server_socket = socket(AF_INET, SOCK_STREAM, 0)), "Failed to create socket");

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(port);

	check(bind(server_socket, (SA *)&server_addr, sizeof(server_addr)), "Bind failed");
	check(listen(server_socket, backlog), "LIsten failed.");
	return server_socket;
}

void *handle_connection(int client_socket)
{
	char buffer[BUFSIZE];
	size_t bytes_read;
	int msgsize = 0;
	char actualpath[PATH_MAX + 1];

	while ((bytes_read = read(client_socket, buffer + msgsize, sizeof((buffer)-msgsize - 1))))
	{
		msgsize += bytes_read;
		if (msgsize > BUFSIZE - 1 || buffer[msgsize - 1] == '\n')
			break;
	}
	check(bytes_read, "recv error");
	buffer[msgsize - 1] = 0;
	printf("REQUEST: %s\n", buffer);
	fflush(stdout);

	if (realpath(buffer, actualpath) == NULL)
	{
		printf("Error(bad path): %s\n", buffer);
		close(client_socket);
		return NULL;
	}

	FILE *fp = fopen(actualpath, "r");
	if (fp == NULL) {

		printf("ERROR(open): %s\n", buffer);
		close(client_socket);
		return NULL;
	}

	while((bytes_read = fread(buffer, 1, BUFSIZE, fp)) > 0)
	{
		write(client_socket, buffer, bytes_read);
	}
	close(client_socket);
	fclose(fp);
	printf("closing connection\n");
	return NULL;

}

int check(int exp, const char *msg)
{
	if (exp == SOCKETERROR)
	{
		perror(msg);
		exit(1);
	}
	return exp;
}

int accept_new_connection(int server_socket)
{
	int addr_size = sizeof(SA_IN);
	int client_socket;
	SA_IN client_addr;
	check(client_socket = accept(server_socket, (SA*)&client_addr, (socklen_t*)&addr_size), "accept failed");
	return client_socket;
}