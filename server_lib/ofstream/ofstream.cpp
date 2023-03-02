/*
 * https://cplusplus.com/reference/fstream/ofstream/
 https://github.com/cclaude42/webserv/blob/master/srcs/response/Response.cpp
 */

#include <fstream>
#include <iostream>

int main() {

	std::string path = "/Users/rburri/Desktop/network_cpp/server_lib/website/submitPage.html";
	std::ifstream mystream;
	mystream.open(path.c_str(), std::ios::binary);
	mystream.seekg(0, std::ios::end);
	int file_size = mystream.tellg();
	std::cout << "file.html length : " << file_size << std::endl;

	mystream.close();

	return 0;
}