#include "Socket.hpp"

Socket::Socket(int domain , int port, int type , int protocol, u_long interface)
{
	// Define address structure
	_address.sin_family = domain;
	_address.sin_port = htons(port);
	_address.sin_addr.s_addr = htonl(interface);

	createSocket(domain, type, protocol);
	establishConnection();
	startListening();

}

void Socket::createSocket(int domain, int type, int protocol)
{
	_sock_id = socket(domain, type, protocol);
	test_socket(_sock_id);
}
void Socket::establishConnection()
{
	// a pointer to a struct sockaddr_in can be cast to a pointer to a struct sockaddr and vice-versa.
	// So even though connect() wants a struct sockaddr*, you can still use a struct sockaddr_in and cast it at the last minute!
	_connection = bind(_sock_id, (struct sockaddr *)&_address, sizeof(_address));
	test_socket(_connection);
}
void Socket::startListening()
{
	int res = listen(_sock_id, 10);
	test_socket(res);
}

void Socket::test_socket(int item_to_test)
{
	if (item_to_test < 0)
	{
		perror("Fail to set up socket");
		exit(EXIT_FAILURE);
	}
}

int Socket::get_sock_id() const
{
	return _sock_id;
}
