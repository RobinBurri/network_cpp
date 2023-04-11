#ifndef SOCKET_HPP
#define SOCKET_HPP

#include "./request.hpp"
#include "./response.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <cstring>
#include <unistd.h>

#include <map>

#define LISTEN_BACKLOG 10
#define RECV_BUFFER 4096

class Socket
{
  private:
	int				   _sock_id;
	int				   _connection;
	std::map<int, std::string>	_requests;
	struct sockaddr_in _address;
	http::Request			  _request_parser;
	http::Response _response_handler;


  public:
  	int				   socket_accept(void);
	int				   socket_recv(int socket);
	void			   create_socket(int domain, int type, int protocol);
	void			   binding_socket();
	void			   start_listening();
	void			   test_socket(int item_to_test, const char *);
	void			   set_socket_non_blocking();
	void			   close(int socket);
	Socket(int domain, unsigned short port, int type, int protocol);
	int			   get_sock_id() const;
	unsigned short get_port() const;
};

#endif
