#include "Socket.hpp"
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>
#include <string>
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"

const int MAXLINE = 1000;

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
	int recv_return;
	HttpRequest requestHandler;
	HttpResponse response_handler;
	std::vector<std::string> header;
	char uniq_response[2000] = "HTTP/1.1 200 OK\nContent-Type:text/html\nContent-Length: 160\n\n<html><head><title>Webserver dummy html page</title></head><body><h1>Welcome to Webserver dummy web page !!!</h1></body></html>";

	while (1)
	{
		std::cout << "++++++ Waiting for new connection ++++++" << std::endl;
		// response_handler.load_response_map(200);
		// response_handler.print_response_map();
		connection_fd = accept(sock.get_sock_id(), (struct sockaddr *)NULL, NULL);
		if (connection_fd < 0)
		{
			std::cout << "connection_fd: " << connection_fd << std::endl;
			perror("Accept connection error");
			exit(EXIT_FAILURE);
		}
		recv_return = recv(connection_fd, buffer, MAXLINE - 1, 0);
		while (recv_return > 0)
		{
			requestHandler.parseBuffer(buffer);
			if (buffer[recv_return - 1] == '\n' || recv_return < MAXLINE)
			{
				break;
			}
			memset(buffer, 0, MAXLINE);
			recv_return = recv(connection_fd, buffer, MAXLINE - 1, 0);
		}
		std::cout << "***************** HTTP REQUEST START****************" << std::endl;
		requestHandler.printHttpReq();
		std::cout << "***************** HTTP REQUEST END ****************" << std::endl;
		std::cout << "***************** HTTP REPONSE START****************" << std::endl;
		response_handler.load_http_request(requestHandler);
		response_handler.print_response_map();
		response_handler.init_response_map();

		std::cout << "***************** HTTP REPONSE END ****************" << std::endl;

		send(connection_fd, uniq_response, sizeof(uniq_response), 0);
		std::cout << "RESPONSE SEND" << std::endl;
		close(connection_fd);
		std::cout << "CONNECTION CLOSED" << std::endl;
	}
	return 0;
}