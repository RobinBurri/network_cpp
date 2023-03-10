#ifndef HTTPRESPONSE_HPP
#define HTTPRESPONSE_HPP

#include "./StatusCode.hpp"
#include <fstream>
#include <string>
#include <time.h>
#include <sstream>
#include "HttpRequest.hpp"

class HttpResponse
{
public:
	typedef std::map<std::string, std::string> t_object;
	t_object _response_map;

	HttpResponse();
	~HttpResponse(void);
	std::string get_http_response(void);
	void load_http_request(HttpRequest &req);

	private : 
	StatusCode _status_code;
	HttpRequest _request;

	void set_content_length(std::string str);
	void init_response_map(void);
	void set_response_type(std::string path);
	bool file_exists(std::string path);
	void load_response_map(int status_code);
	std::string get_time_stamp(void);
	void construct_header_string(void);
	void construct_body_string(std::string path_to_file);
	void construct_full_response(void);
	void create_error_html_page(int code);
};

std::ostream &operator<<(std::ostream &, HttpResponse const &);

#endif