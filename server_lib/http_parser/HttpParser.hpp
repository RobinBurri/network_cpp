#ifndef HTTPPARSER_HPP
#define HTTPPARSER_HPP

#include <map>
#include <vector>
#include <string>
#include  <cstring> 
#include <bits/stdc++.h>
// to delete later
#include <iostream>


class HttpParser
{
private:
	std::map<std::string, std::string> _http_req;
public:
	HttpParser();
	~HttpParser();
	void parseBuffer(char *buff);
	void HttpParser::vectorToMap(std::vector < std::string>);
};

#endif