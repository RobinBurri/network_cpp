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

	char test_str[2000] = "HTTP/1.1 200 OK\nDate:Fri Mar  3 11:40:35 2023 GMT\nServer:Webserver\nContent-Length: 769\nContent-Type:text/html\nConnection:close\n\n<!DOCTYPE html>\n<html lang='en'>\n<head>\n<meta charset='UTF-8' />\n<meta http-equiv='X-UA-Compatible'content='IE=edge' />\n<meta name='viewport' content='width=device-width, initial-scale=1.0' />\n<link rel='stylesheet' href='style.css' />\n<title>Kill your eyes</title>\n</head>\n<body>\n<header>\n<div class='wrapper nav'>\n<div class='logo'><h2>Test Website</h2></div>\n<div class='navigation'>\n<a href='./index.html'>Home</a><a href='./submitPage.html'>Submit</a>\n</div>\n</div>\n</header>\n<main class='index-main'>\n<div class='wrapper'>\n<div class='centered-box'>\n<h1 class='title'>Welcome Tester Website</h1>\n</div>\n</div>\n</main>\n</body>\n</html>";

	// char uniq_response[2000] = "HTTP/1.1 200 OK\nContent-Type:text/html\nContent-Length: 160\n\n<html><head><title>Webserver dummy html page</title></head><body><h1>Welcome to Webserver dummy web page !!!</h1></body></html>";
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
		std::cout << "***************** HTTP REQUEST END ****************\n" << std::endl;
	
		response_handler.load_http_request(requestHandler);
		// std::cout << "***************** HTTP REPONSE START****************" << std::endl;
		// response_handler.print_response_map();
		// std::cout << "***************** HTTP REPONSE END ****************\n" << std::endl;
	
		std::string res = response_handler.get_http_response();
		std::cout << "$$$$$$$$$$$$$$$$$$$$$$$$$" << std::endl;
		std::cout << res;
		std::cout << "$$$$$$$$$$$$$$$$$$$$$$$$$" << std::endl;
		// send(connection_fd, res.c_str(), sizeof(res.c_str()), 0);
		send(connection_fd, test_str, sizeof(test_str), 0);
		// send(connection_fd, uniq_response, sizeof(uniq_response), 0);
		std::cout << "RESPONSE SEND" << std::endl;
		close(connection_fd);
		std::cout << "CONNECTION CLOSED" << std::endl;
	}
	return 0;
}

/*
	! index.html = 769 (httpResponse = 769)
	! 404.html = 380  (httpResponse = 308)
	! submitPage.html = 1006  (httpResponse = 1006)
*/