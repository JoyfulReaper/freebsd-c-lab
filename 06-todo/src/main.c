#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "todo.h"

static void print_usage(const char *program)
{
	fprintf(stderr, "Usage:\n%s add <title> <description>\n", program);
	fprintf(stderr, "%s list\n", program);
}

int main(int argc, char *argv[])
{
	if(argc < 2)
	{
		print_usage(argv[0]);
		return EXIT_FAILURE;
	}
	
	if(strcmp(argv[1], "add") != 0 &&
	   strcmp(argv[1], "list") != 0) {
		print_usage(argv[0]);
		return EXIT_FAILURE;
	}
	
	if(strcmp(argv[1], "add") == 0)
	{
		if(argc != 4) {
			print_usage(argv[0]);
			return EXIT_FAILURE;
		}
		
		struct todo todo = {
			.id = 0,
			.title = argv[2],
			.description = argv[3],
			.completed = false
		};
		
		if(todo_add(&todo) != 0) {
			fprintf(stderr, "Failed to add todo!");
			return EXIT_FAILURE;
		}
		else
		{
			printf("Todo Added!\n");
		}
	}
	else if(strcmp(argv[1], "list") == 0)
	{
		if(argc != 2)
		{
			print_usage(argv[0]);
			return EXIT_FAILURE;
		}
		
		if(todo_list() != 0) {
			fprintf(stderr, "Failed to list todos!");
			return EXIT_FAILURE;
		}
	}
	else
	{
			print_usage(argv[0]);
			return EXIT_FAILURE;
	}


	return EXIT_SUCCESS;
}
