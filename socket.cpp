#include "socket.hpp"

Socket::Socket(int domain, unsigned short port, int type, int protocol)
{
	_address.sin_family = domain;
	_address.sin_port = htons(port);
	_address.sin_addr.s_addr = htonl(INADDR_ANY);

	create_socket(domain, type, protocol);
	set_socket_non_blocking();
	binding_socket();
	start_listening();
}

void Socket::create_socket(int domain, int type, int protocol)
{
	_sock_id = socket(domain, type, protocol);
	test_socket(_sock_id, "create_socket() Fail!");
}

void Socket::binding_socket()
{
	_connection = bind(_sock_id, reinterpret_cast<struct sockaddr *>(&_address), sizeof(_address));
	test_socket(_connection, "binding_socket() Fail!");
}

void Socket::start_listening()
{
	int res;
	res = listen(_sock_id, LISTEN_BACKLOG);
	test_socket(res, "start_listening() Fail!");
}

void Socket::test_socket(int item_to_test, const char *msg)
{
	if (item_to_test < 0)
	{
		close(_sock_id);
		throw std::runtime_error(msg);
	}
}

int Socket::get_sock_id() const
{
	return _sock_id;
}

unsigned short
Socket::get_port() const
{
	return ntohs(_address.sin_port);
}

void Socket::set_socket_non_blocking()
{
	int ret;
	int val = 1;
	ret = setsockopt(_sock_id, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));
	test_socket(ret, "setsockopt() Fail!");

	// Following code only working after select() is implemented
	// ret = fcntl(_sock_id, F_SETFL, O_NONBLOCK);
	// test_socket(ret, "fcnt() Fail!");
}

int Socket::socket_accept(void)
{
	int socket;

	socket = accept(_sock_id, NULL, NULL);
	if (socket == -1)
		std::cerr << "Could not create socket. " << std::endl;
	// clean and stop? => not stop but restart?
	else
	{
		_requests.insert(std::make_pair(socket, ""));
	}
	return (socket);
}

// Do the loop for reading request.
int Socket::socket_recv(int socket)
{
	char buffer[RECV_BUFFER] = {0};
	int ret;

	ret = recv(socket, buffer, sizeof(RECV_BUFFER - 1), 0);

	if (ret == -1)
	{
		this->close(socket);
		std::cout << "\rRead error, closing connection.\n"
				  << std::endl;
		return (-1);
	}

	_requests[socket] += std::string(buffer);
	if (_requests[socket].find("Transfer-Encoding: chunked") != std::string::npos && ret == RECV_BUFFER - 1)
	{
		std::memset(buffer, 0, sizeof(RECV_BUFFER));
		ret = recv(socket, buffer, sizeof(RECV_BUFFER - 1), 0);
		if (ret == -1)
		{
			this->close(socket);
			std::cout << "\rRead error, closing connection.\n"
					  << std::endl;
			return (-1);
		}
		_requests[socket] += std::string(buffer);
	}
	_request_parser.parse_buffer(_requests[socket]);
	return 0;

	// size_t i = _requests[socket].find("\r\n\r\n");
	// if (i != std::string::npos)
	// {
	// 	if (_requests[socket].find("Content-Length: ") == std::string::npos)
	// 	{
	// 		if (_requests[socket].find("Transfer-Encoding: chunked") != std::string::npos)
	// 		{
	// 			// if (checkEnd(_requests[socket], "0\r\n\r\n") == 0)
	// 			// 	return (0);
	// 			// else
	// 			// 	return (1);
	// 		}
	// 		else
	// 			return (0);
	// 	}

	// 	size_t len = std::atoi(_requests[socket].substr(_requests[socket].find("Content-Length: ") + 16, 10).c_str());
	// 	if (_requests[socket].size() >= len + i + 4)
	// 		return (0);
	// 	else
	// 		return (1);
	// }

	// return (1);
}

void Socket::close(int socket)
{
	if (socket > 0)
		::close(socket);
	_requests.erase(socket);
}