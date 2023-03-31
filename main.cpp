#include "Socket.hpp"
#include <vector>
#include "request.hpp"
#include "response.hpp"
// select includes:
#include <sys/time.h>
#include <sys/types.h>
#include <cstdio>
#include <sys/ioctl.h>
#include <sys/time.h>

#define SERVER_PORT 12345
#define BUFFER_SIZE 256

#define TRUE 1
#define FALSE 0

int main()
{
	int i, len, rc = 1;
	int select_ret = 0;
	int listen_sd1, listen_sd2, max_sd, new_sd;
	int desc_ready, end_server = FALSE;
	int close_conn;
	char buffer[BUFFER_SIZE];
	char buffer_null_terminated[BUFFER_SIZE + 1];
	struct fd_set master_set, working_set;
	Socket socket1(AF_INET, 8080, SOCK_STREAM, 0);
	Socket socket2(AF_INET, 9090, SOCK_STREAM, 0);

	listen_sd1 = socket1.get_sock_id();
	listen_sd2 = socket2.get_sock_id();
	FD_ZERO(&master_set);
	/* FD_ZERO() clears out the master_set called socks, so that
		it doesn't contain any file descriptors. */
	max_sd = listen_sd2;
	FD_SET(listen_sd1, &master_set);
	FD_SET(listen_sd2, &master_set);
	/* FD_SET() adds the file descriptor "sock" to the master_set,
		so that select() will return if a connection comes in
		on that socket (which means you have to do accept(). */

	/* Loop waiting for incoming connects or for incoming data
		on any of the connected sockets. */
	do
	{
		memcpy(&working_set, &master_set, sizeof(master_set));
		printf("Waiting on select()...\n");
		std::cout << "max_sd: " << max_sd << std::endl;

		select_ret = select(max_sd + 1, &working_set, NULL, NULL, NULL);
		if (select_ret < 0)
		{
			perror("  select() failed");
			break;
		}

		/* One or more descriptors are readable.  Need to
			determine which ones they are. */
		desc_ready = select_ret;
		for (i = 0; i <= max_sd && desc_ready > 0; ++i)
		{
			/* Check to see if this descriptor is ready */
			if (FD_ISSET(i, &working_set))
			{
				/* A descriptor was found that was readable - one
					less has to be looked for.  This is being done
					so that we can stop looking at the working set
					once we have found all of the descriptors that
					were ready. */
				desc_ready -= 1;

				/* Check to see if this is the listening socket */
				if (i == listen_sd2 || i == listen_sd1)
				{
					printf("  Listening socket is readable\n");
					/* Accept all incoming connections that are
						queued up on the listening socket before we
						loop back and call select again. */
					do
					{
						/* Accept each incoming connection.  If
							accept fails with EWOULDBLOCK, then we
							have accepted all of them.  Any other
							failure on accept will cause us to end the
							server.                                    */
						new_sd = accept(i, NULL, NULL);
						if (new_sd < 0)
						{
							if (errno != EWOULDBLOCK)
							{
								perror("  accept() failed");
								end_server = TRUE;
							}
							break;
						}

						/* Add the new incoming connection to the
							master read set	*/
						printf("  New incoming connection - %d\n", new_sd);
						FD_SET(new_sd, &master_set);
						if (new_sd > max_sd)
							max_sd = new_sd;
					} while (new_sd != -1);
				}

				/* This is not the listening socket, therefore an 
				existing connection must be readable */
				else
				{
					http::Request req;
					http::Response res;
					std::string str_buff = "";
					printf("  Descriptor %d is readable\n", i);
					close_conn = FALSE;

					/* Receive all incoming data on this socket
						before we loop back and call select again. */
					do
					{


						/* Receive data on this connection until the 
							recv fails with EWOULDBLOCK.  If any other 
							failure occurs, we will close the
							connection.  */

						rc = recv(i, buffer, sizeof(buffer), 0);
						if (rc < 0)
						{
							if (errno != EWOULDBLOCK)
							{
								perror("  recv() failed");
								close_conn = TRUE;
							}
							break;
						}

						/* Check to see if the connection has been
							closed by the client  */
						if (rc == 0)
						{
							printf("  Connection closed\n");
							close_conn = TRUE;
							break;
						}

						/* Data was received  */
						if (rc > 0)
						{
							memcpy(buffer_null_terminated, buffer, rc);
							buffer_null_terminated[rc] = '\0';
							str_buff += buffer_null_terminated;
						}
						len = rc;
						printf("  %d bytes received\n", len);

					} while (TRUE);
					req.parse_buffer(str_buff);
					std::cout << "***************** HTTP REQUEST START****************" << std::endl;
					std::cout << req << std::endl;
					std::cout << "***************** HTTP REQUEST END ****************\n";

					res.load_http_request(req);
					// std::cout << "***************** HTTP REPONSE START****************" << std::endl;
					// std::cout << res << std::endl;
					// std::cout << "***************** HTTP REPONSE END ****************\n" << std::endl;
					std::string response = res.get_http_response();
					int send_ret = send(i, response.c_str(), response.length(), 0);
					if (send_ret < static_cast<int>(response.length()))
					{
						std::cout << "send_ret : " << send_ret << std::endl;
						send_ret = send(i, response.c_str(), response.length(), 0);
					}

					/* If the close_conn flag was turned on, we need
						to clean up this active connection.  This    
						clean up process includes removing the       
						descriptor from the master set and           
						determining the new maximum descriptor value 
						based on the bits that are still turned on in
						the master set.                               */
					if (close_conn)
					{
						close(i);
						FD_CLR(i, &master_set);
						if (i == max_sd)
						{
							while (FD_ISSET(max_sd, &master_set) == FALSE)
								max_sd -= 1;
						}
					}
				} /* End of existing connection is readable */
			}	  /* End of if (FD_ISSET(i, &working_set)) */
		}		  /* End of loop through selectable descriptors */

	} while (end_server == FALSE);

	/* Clean up all of the sockets that are open                 */
	for (i = 0; i <= max_sd; ++i)
	{
		if (FD_ISSET(i, &master_set))
			close(i);
	}
}
