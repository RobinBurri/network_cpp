#include "./HttpRequest.hpp"

HttpRequest::HttpRequest(){};
HttpRequest::~HttpRequest(){};

void HttpRequest::parseBuffer(char *buff)
{
	std::vector<std::string> tmpVector;
	std::string str_buff = buff;
	std::string delimiter = "\r\n";
	std::string str;
	int delimiter_position = str_buff.find(delimiter);
	while (delimiter_position != -1)
	{
		str = str_buff.substr(0, delimiter_position);
		tmpVector.push_back(str);
		str_buff.erase(str_buff.begin(), str_buff.begin() + delimiter_position + 1);
		delimiter_position = str_buff.find(delimiter);
	}
	this->parseFirstLine(tmpVector[0]);
	this->parseOtherLines(tmpVector);
};

void HttpRequest::parseFirstLine(std::string firstLine)
{
	std::vector<std::string> tmpVector;
	std::string delimiter = " ";
	std::string str;
	int delimiter_position = firstLine.find(delimiter);
	while (delimiter_position != -1)
	{
		str = firstLine.substr(0, delimiter_position);
		tmpVector.push_back(this->trim(str));
		firstLine.erase(firstLine.begin(), firstLine.begin() + delimiter_position + 1);
		delimiter_position = firstLine.find(delimiter);
	}
	tmpVector.push_back(this->trim(firstLine));

	if (tmpVector.size() != 3)
		return;
	_http_req["Method"] = tmpVector[0];
	_http_req["Path"] = tmpVector[1];
	_http_req["Protocol"] = tmpVector[2];
};

void HttpRequest::parseOtherLines(std::vector<std::string> tmpVector)
{
	std::string delimiter = ":";
	std::string key;
	std::string value;
	int delimiter_position;
	std::cout << "tmpVector.size() = " << tmpVector.size() << std::endl;
	for (size_t i = 1; i < tmpVector.size(); i++)
	{
		std::cout << "tmpVector[" << i << "] = " << tmpVector[i] << std::endl;
		if (i == tmpVector.size() - 1)
		{
			std::cout << "Last line : " << tmpVector[i] << std::endl;
		}
		delimiter_position = tmpVector[i].find(delimiter);
		key = tmpVector[i].substr(0, delimiter_position);
		value = tmpVector[i].substr(delimiter_position + 1);
		if (HttpRequest::trim(key).length() != 0 || HttpRequest::trim(value).length() != 0)
			_http_req[HttpRequest::trim(key)] = HttpRequest::trim(value);
	}
	std::cout << "Does this function finish?" << std::endl;
};

std::string HttpRequest::trim(const std::string &s)
{
	const std::string _WHITESPACE = " \n\r\t\f\v";
	std::string leftTrimedString = "";
	size_t start;
	size_t end;
	start = s.find_first_not_of(_WHITESPACE);
	if (start == std::string::npos)
	{
		return leftTrimedString;
	}
	else
	{
		leftTrimedString = s.substr(start);
	}
	end = leftTrimedString.find_last_not_of(_WHITESPACE);
	return leftTrimedString.substr(0, end + 1);
}

void HttpRequest::printHttpReq()
{
	std::map<std::string, std::string>::iterator it;
	for (it = _http_req.begin(); it != _http_req.end(); it++)
	{
		std::cout << it->first << " : " << it->second << std::endl;
	}
};

std::string HttpRequest::getMethod() const
{
	return _http_req.at("Method");
};

std::string HttpRequest::getPath() const
{
	return _http_req.at("Path");
};

std::string HttpRequest::getProtocol() const
{
	return _http_req.at("Protocol");
};

std::string HttpRequest::getHost() const
{
	return _http_req.at("Host");
};

bool HttpRequest::methodIsAuthorized(std::string method) const
{
	return (method.compare("GET") || method.compare("POST") || method.compare("DELETE"));
}