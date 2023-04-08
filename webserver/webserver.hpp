#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

#include <sys/socket.h> // socket
#include <netinet/in.h> // struct sockaddr_in
#include <arpa/inet.h>	// inet_pton
#include <stdarg.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define SERVER_PORT 18000

#define MAXLINE 4096
#define SA struct sockaddr

void err_n_die(const char *fmt, ...)
{
	int errno_save;
	va_list ap;

	errno_save = errno;

	va_start(ap, fmt);
	vfprintf(stdout, fmt, ap);
	fprintf(stdout, "\n");
	fflush(stdout);

	if (errno_save != 0)
	{
		fprintf(stdout, "(errno = %d) : %s\n", errno_save, strerror(errno_save));
		fprintf(stdout, "\n");
		fflush(stdout);
	}

	va_end(ap);

	exit(1);
}

char *bin2hex(const unsigned char *input, size_t len)
{
	char *result;
	char *hexits = "0123456789ABCDF";

	if (input == NULL || len <= 0)
		return NULL;

	int result_length = (len * 3) + 1;

	printf("result_length: %d\n", result_length);
	result = (char *)malloc(result_length);
	bzero(result, result_length);
	for (size_t i = 0; i < len; i++)
	{
		result[i * 3] = hexits[input[i] >> 4];
		result[(i * 3) + 1] = hexits[input[i] & 0x0F];
		result[(i * 3) + 2] = ' ';
	}
	return result;
}
#endif
