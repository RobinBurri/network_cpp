#ifndef HTTPPARSER_HPP
#define HTTPPARSER_HPP

#include <string.h>
#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>

class HttpParser
{
private:
	std::unordered_map<std::string, std::string> _http_req;

public:
	HttpParser();
	~HttpParser();
	void parseBuffer(char *buff);
	void parseFirstLine(std::string firstLine);
	void parseOtherLines(std::vector<std::string> tmpVector);
	std::string trim(const std::string &s);
	void print_http_req();
};

#endif