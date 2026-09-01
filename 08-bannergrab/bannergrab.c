#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <inttypes.h>
#include <errno.h>

void print_usage(char *program)
{
	fprintf(stderr, "Usage: %s <host> <port>\n", program);
}

bool process_arguments(int argc, char *argv[], char **host, uint16_t *port)
{
	if(argc != 3)
	{
		return false;
	}
	
	*host = argv[1];
	
	char *end;
	long value = strtol(argv[2], &end, 10);
	if(value < 1 || value > 65535 || errno == ERANGE)
		return false;
	if(end == argv[2] || *end != '\0')
		return false;
		
	*port = (uint16_t)value;
	
	return true;
}

int main(int argc, char *argv[])
{
	uint16_t port = 0;
	char *host;
	
	if(!process_arguments(argc, argv, &host, &port))
	{
		print_usage(argv[0]);
		return EXIT_FAILURE;
	}
	
	printf("Host: %s\n", host);
	printf("Port: %"PRIu16"\n", port);
	
	return EXIT_SUCCESS;
}
