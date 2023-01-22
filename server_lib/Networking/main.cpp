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


	while (1)
	{
		std::cout << "++++++ Waiting for new connection ++++++" << std::endl;
		connection_fd = accept(sock.get_sock_id(), (struct sockaddr *)NULL,NULL);
		if (connection_fd < 0)
		{
			std::cout << "connection_fd: " << connection_fd << std::endl;
			perror("Accept connection error");
			exit(EXIT_FAILURE);
		}
		read_return = read(connection_fd, buffer, MAXLINE - 1);
		while (read_return > 0)
		{
			std::cout << "buffer: " << buffer << std::endl;
			if (buffer[read_return -1 ] == '\n') 
			{
				break;
			}
			read_return = read(connection_fd, buffer, MAXLINE - 1);
		}
		write(connection_fd, "hello", 6);
		std::cout << "message send" << std::endl;
		close(connection_fd);
	}

	return 0;
}