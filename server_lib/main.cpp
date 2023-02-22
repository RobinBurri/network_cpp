#include "Socket.hpp"
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>
#include <string>
#include "HttpParser.hpp"

const int MAXLINE = 10240;

int main()
{
	/*
	 *  sock @params:
	 *	AF_INET for IPv4,
	 *	Port number,
	 *	SOCK_STREAM bidirectional byte-stream socket,
	 *	AF_INET + SOCK_STREAM always protocol = 0,
	 *	address for this socket, let the OS choose = INADDR_ANY
	 */
	Socket sock(AF_INET, 8080, SOCK_STREAM, 0, INADDR_ANY);
	int connection_fd;
	char buffer[MAXLINE] = {0};
	int read_return;
	HttpParser req_parser;
	std::vector<std::string> header;
	char uniq_response[2000] = "HTTP/1.1 200 OK\nContent-Type:text/html\nContent-Length: 160\n\n<html><head><title>Webserver dummy html page</title></head><body><h1>Welcome to Webserver dummy web page !!!</h1></body></html>";

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

				// std::cout << "HTTP request:\n"
				// 		  << buffer << std::endl;
			req_parser.parseBuffer(buffer);
			if (buffer[read_return - 1] == '\n')
			{
				break;
			}
			read_return = read(connection_fd, buffer, MAXLINE - 1);
		}
		std::cout << "header.size(): " << header.size() << std::endl;
 		for (unsigned int i= 0; i < header.size(); i++) {
			std::cout << header[i] << std::endl;
		}
		req_parser.print_http_req();
		send(connection_fd, uniq_response, sizeof(uniq_response), 0);
		std::cout << "RESPONSE SEND" << std::endl;
		close(connection_fd);
		std::cout << "CONNECTION CLOSED" << std::endl;
	}

	return 0;
}