#ifndef STATUSCODE_HPP
#define STATUSCODE_HPP

#include <map>
#include <string>
#include <iostream>

class StatusCode
{
private:
	std::map<std::string, std::string> _status_codes;

public:
	StatusCode();
	std::string &operator[](std::string status_code);
};

#endif