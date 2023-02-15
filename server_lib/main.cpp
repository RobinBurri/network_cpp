#include "Socket.hpp"
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

const int MAXLINE = 1024;

int main()
{

	Socket sock(AF_INET, 8080, SOCK_STREAM, 0, INADDR_ANY);
	int connection_fd;
	char buffer[MAXLINE] = {0};
	int read_return;
	std::string ok_resp = "HTTP/1.1 200 OK";

	while (1)
	{
		std::cout << "++++++ Waiting for new connection ++++++" << std::endl;
		connection_fd = accept(sock.get_sock_id(), (struct sockaddr *)NULL, NULL);
		if (connection_fd < 0)
		{
			std::cout << "connection_fd: " << connection_fd << std::endl;
			perror("Accept connection error");
			exit(EXIT_FAILURE);
		}
		read_return = read(connection_fd, buffer, MAXLINE - 1);
		while (read_return > 0)
		{
			std::cout << "HTTP request:\n"
					  << buffer << std::endl;
			if (buffer[read_return - 1] == '\n')
			{
				break;
			}
			read_return = read(connection_fd, buffer, MAXLINE - 1);
		}
		char arr[2000] = "HTTP/1.1 200 OK\nContent-Type:text/html\nContent-Length: 160\n\n<html><head><title>Webserver dummy html page</title></head><body><h1>Welcome to Webserver dummy html page !!!</h1></body></html>";
						 int send_res = send(connection_fd, arr, sizeof(arr), 0);
		std::cout << "response send" << std::endl;
		close(connection_fd);
		std::cout << "connection closed" << std::endl;
	}

	return 0;
}