#include "cluster.hpp"

void setup()
{
	
}


void Cluster::load_config()
{
	// we will pass json::t_object *config to this function
	// and call value get method to create all necessary sockets

	// For now only:
	for (int i = 0 ; i < 5; i++)
	{
		t_host_port new_server;
		new_server.port = 8080 + i;
		new_server.host = "";
		new_server.path = "";
		_servers.push_back(new_server);
	} 
}

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

	_servers = src._servers;
	return (*this);
}