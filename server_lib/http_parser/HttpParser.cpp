#include "HttpParser.hpp"

HttpParser::HttpParser() {};
HttpParser::~HttpParser() {};

void HttpParser::parseBuffer(char * buff) {
	std::vector<std::string> tmpVector;
	std::string buff_str = buff;
	std::string token;
	std::string delimiter = "\n";
	size_t pos  = 0;
	while ((pos = buff_str.find(delimiter)) != std::string::npos)
	{
		token = buff_str.substr(0, pos);
		std::cout << "token : " << token << std::endl;
		tmpVector.push_back(token);
		buff_str.erase(0, pos + delimiter.length());
	}
};

void HttpParser::vectorToMap(std::vector<std::string> tokenVector) {
	char * token = new char[tokenVector[0].length() +1];
	strcpy(token, tokenVector[0].c_str());
	char *splitedToken = strtok(token, " ");

	_http_req["Method"] = splitedToken[0];
	_http_req["Request Target"] = splitedToken[1];
	_http_req["Protocol"]  = splitedToken[2];
	for (int i = 1; i < tokenVector.size(); i++) {
		std::stringstream f(tokenVector[i]);
		std::string part;
		while (getline(f, part, ':'))
		{
			
		
		}
	}
};