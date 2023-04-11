#include "cluster.hpp"

// Cluster::Cluster(void) : _config(DEFAULT_CONFIG)
// {
// }

Cluster::Cluster(void)
{
}

Cluster::Cluster(const Cluster &src)
{
	if (this != &src)
	{
		*this = src;
	}
}

Cluster::~Cluster(void)
{
}

Cluster &Cluster::operator=(const Cluster &src)
{
	_server_vector = src._server_vector;
	_sockets = src._sockets;
	_fd_set = src._fd_set;
	_max_fd = src._max_fd;
	return (*this);
}

void Cluster::load_config()
{
	// we will pass json::t_object *config to this function
	// and call value get method to create all necessary sockets

	// For now only:
	for (int i = 0; i < 5; i++)
	{
		t_host_port new_server;
		new_server.port = 8080 + i;
		new_server.host = "";
		new_server.path = "";
		_server_vector.push_back(new_server);
	}
}

void Cluster::setup()
{
	FD_ZERO(&_fd_set);
	_max_fd = 0;

	for (std::vector<t_host_port>::iterator it(_server_vector.begin()); it != _server_vector.end(); it++)
	{
		try
		{
			int fd;
			Socket new_socket(AF_INET, it->port, SOCK_STREAM, 0);
			fd = new_socket.get_sock_id();
			FD_SET(fd, &_fd_set);
			_sockets.insert(std::make_pair(fd, &new_socket));
			_sockets_fds.push_back(fd);
			if (fd > _max_fd)
				_max_fd = fd;
			std::cout << it->host << ":" << it->port << " set up" << std::endl;
		}
		catch (std::exception)
		{
			exit(1);
		}
	}
}

void Cluster::run()
{
	while (true)
	{
		fd_set reading_set;
		fd_set writing_set;
		int select_return = 0;

		while (select_return == 0)
		{
			std::memcpy(&reading_set, &_fd_set, sizeof(_fd_set));
			FD_ZERO(&writing_set);
			for (std::map<int, Socket *>::iterator it(_sockets.begin()); it != _sockets.end(); it++)
				FD_SET(it->first, &writing_set);
			std::cout << "Waiting on a connection" << std::endl;
			select_return = select(_max_fd + 1, &reading_set, &writing_set, NULL, NULL);
		}
		if (select_return)
		{
			// one or more socket_fd is ready to be read.
			for (std::map<int, Socket *>::iterator it(_sockets.begin()); select_return && it != _sockets.end(); it++)
			{
				int tested_socket = it->first;

				if (FD_ISSET(tested_socket, &reading_set))
				{
					int	ret = it->second->socket_recv(tested_socket);

					if (ret == 0)
					{
						it->second->process(tested_socket, _config);
						_ready.push_back(tested_socket);
					}
					else if (ret == -1)
					{
						FD_CLR(tested_socket, &_fd_set);
						FD_CLR(tested_socket, &reading_set);
						_sockets.erase(tested_socket);
						it = _sockets.begin();
					}
					ret = 0;
					break;
				}
			}
			// 	for (std::map<long, Server>::iterator it = _servers.begin() ; ret && it != _servers.end() ; it++)
			// {
			// 	long	fd = it->first;

			// 	if (FD_ISSET(fd, &reading_set))
			// 	{
			// 		long	socket = it->second.accept();

			// 		if (socket != -1)
			// 		{
			// 			FD_SET(socket, &_fd_set);
			// 			_sockets.insert(std::make_pair(socket, &(it->second)));
			// 			if (socket > _max_fd)
			// 				_max_fd = socket;
			// 		}
			// 		ret = 0;
			// 		break;
			// 	}
			// }
		}
		else
		{
			// problem with select, cleaning up.
			std::cerr << "Error with select()" << std::endl;
			close_all_sockets();
			_server_vector.clear();
			_sockets.clear();
			_sockets_fds.clear();
		}
	}
}

// Close every socket fd that are open
void Cluster::close_all_sockets()
{
	for (std::vector<int>::iterator it(_sockets_fds.begin()); it != _sockets_fds.end(); ++it)
	{
		if (*it > 0)
			close(*it);
		*it = -1;
	}
}
