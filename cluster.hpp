#ifndef CLUSTER_HPP
#define CLUSTER_HPP

#include "Socket.hpp"
#include <iostream>
#include <string>
#include <vector>

class Cluster
{
	Cluster(void);
	Cluster(const Cluster &src);
	~Cluster(void);
	Cluster &operator=(const Cluster &src);

	void load_config();
	void setup();
	// void run();
	private:

		typedef struct host_port
		{
			double port;
			std::string host;
			std::string path;
		} t_host_port;

		std::vector<t_host_port> _servers;
		std::vector<Socket> _socket_ids;
};

#endif
