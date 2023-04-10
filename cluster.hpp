#ifndef CLUSTER_HPP
#define CLUSTER_HPP

#include "socket.hpp"
#include <iostream>
#include <cstring>
#include <vector>
#include <map>

class Cluster
{
	typedef struct host_port
	{
		double port;
		std::string host;
		std::string path;
	} t_host_port;
	Cluster(void);
	Cluster(const Cluster &src);
	~Cluster(void);
	Cluster &operator=(const Cluster &src);

	void load_config();
	void setup();
	void run();
	void close_all_sockets();
	std::vector<t_host_port> _server_vector;
	std::map<int, Socket *> _sockets;
	std::vector<int> _sockets_fds;
	fd_set _fd_set;
	int _max_fd;
};

#endif
