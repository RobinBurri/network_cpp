#ifndef HTTPREQUEST_HPP
#define HTTPREQUEST_HPP

#include <iostream>
#include <map>
#include <string.h>
#include <string>
#include <vector>

class HttpRequest
{
public:
	typedef std::map<std::string, std::string> t_object;

	HttpRequest();
	~HttpRequest();

	void parse_buffer(char *buff);
	bool method_is_authorzed(std::string method) const;
	// Getters
	std::string get_method() const;
	std::string get_path() const;
	std::string get_protocol() const;
	std::string get_host() const;
	const t_object &get_map() const;

private:
	t_object _request_map;
	std::string trim(const std::string &s);
	void parse_first_line(std::string firstLine);
	void parse_other_lines(std::vector<std::string> tmp_vector);
};

std::ostream &operator<<(std::ostream &, HttpRequest const &);

#endif