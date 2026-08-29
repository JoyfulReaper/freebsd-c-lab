#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "todo.h"

static void print_usage(const char *program)
{
	fprintf(stderr, "Usage: %s add <title> <description>\n", program);
}

int main(int argc, char *argv[])
{
	if(argc != 4) {
		print_usage(argv[0]);
		return EXIT_FAILURE;
	}
	
	if(strcmp(argv[1], "add") != 0) {
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
	
	printf("Todo Added!\n");

	return EXIT_SUCCESS;
}
