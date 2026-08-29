#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>

bool process_arguments(int argc, char *argv[])
{
	if(argc != 3)
	{
		fprintf(stderr, "Usage: %s <server> <port>\n", argv[0]);
		return false;
	}
	
	return true;
}

int do_connect(const char *host, const char *port)
{
	struct addrinfo hints, *result, *rp;
	int error, sfd;
	
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	
	error = getaddrinfo(host, port, &hints, &result);
	if(error != 0)
	{
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(error));
		return -1;
	}
	
	for (rp = result; rp != NULL; rp = rp->ai_next)
	{
		sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
		if(sfd == -1)
			continue;
			
		if(connect(sfd, rp->ai_addr, rp->ai_addrlen) == 0)
		{
			printf("Connected using: %s\n", rp->ai_family == AF_INET ? "IPv4" : "IPv6");
            break;
		}
		
		close(sfd);
	}
	
	if(rp == NULL)
	{
		fprintf(stderr, "failed to connect\n");
		freeaddrinfo(result);
		return -1;
	}
	
	freeaddrinfo(result);
	return sfd;
}

int main(int argc, char *argv[])
{
	if(!process_arguments(argc, argv))
		return EXIT_FAILURE;
		
	int sfd = do_connect(argv[1], argv[2]);
	if(sfd == -1)
		return EXIT_FAILURE;
		
	ssize_t bytes_read;
	size_t total_read = 0;
	char buffer[4096];
	
	while((bytes_read = recv(sfd, buffer +  total_read, sizeof buffer - total_read - 1, 0)) > 0)
	{
		total_read += (size_t)bytes_read;
		if(total_read == sizeof buffer -1)
			break;
	}
	
	if(bytes_read == -1)
	{
		perror("recv");
		close(sfd);
		return EXIT_FAILURE;
	}
	close(sfd);
	buffer[total_read] = '\0';
	
	printf("tcp server's response: %s\n", buffer);
		
	return EXIT_SUCCESS;
}
