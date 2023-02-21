#ifndef STATUSCODE_HPP
#define STATUSCODE_HPP

#include <map>
#include <string>

class StatusCode
{
private:
	std::map<int, std::string> _status_codes;

public:
	StatusCode();
	std::string &operator[](int status_code);
};

#endif