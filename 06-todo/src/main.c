#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "todo.h"

static void print_usage(const char *program)
{
	fprintf(stderr, "Usage:\n%s add <title> <description>\n", program);
	fprintf(stderr, "%s list\n", program);
	fprintf(stderr, "%s complete <id>\n", program);
	fprintf(stderr, "%s uncomplete <id>\n", program);
	fprintf(stderr, "%s delete <id>\n", program);
}

int main(int argc, char *argv[])
{
	if(argc < 2)
	{
		print_usage(argv[0]);
		return EXIT_FAILURE;
	}
	
	if(strcmp(argv[1], "add") != 0 &&
	   strcmp(argv[1], "list") != 0 &&
	   strcmp(argv[1], "complete") != 0 &&
	   strcmp(argv[1], "uncomplete") != 0 &&
	   strcmp(argv[1], "delete") != 0) 
	{
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
			fprintf(stderr, "Failed to add todo!\n");
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
			fprintf(stderr, "Failed to list todos!\n");
			return EXIT_FAILURE;
		}
	}
	else if(strcmp(argv[1], "complete") == 0)
	{
		if(argc != 3)
		{
			print_usage(argv[0]);
			return EXIT_FAILURE;
		}
		
		unsigned long id = strtoul(argv[2], NULL, 10);
		if(id == 0)
		{
			fprintf(stderr, "Failed to parse id\n");
			return EXIT_FAILURE;
		}
		
		if(todo_complete(id, true) != 0)
		{
			fprintf(stderr, "Failed to complete todo\n");
			return EXIT_FAILURE;
		}
		
		printf("Completed todo!\n");
	}
	else if(strcmp(argv[1], "uncomplete") == 0)
	{
		if(argc != 3)
		{
			print_usage(argv[0]);
			return EXIT_FAILURE;
		}
		
		unsigned long id = strtoul(argv[2], NULL, 10);
		if(id == 0)
		{
			fprintf(stderr, "Failed to parse id\n");
			return EXIT_FAILURE;
		}
		
		if(todo_complete(id, false) != 0)
		{
			fprintf(stderr, "Failed to uncomplete todo\n");
			return EXIT_FAILURE;
		}
		
		printf("Uncompleted todo!\n");
	}
	else if (strcmp(argv[1], "delete") == 0)
	{
		if(argc != 3)
		{
			print_usage(argv[0]);
			return EXIT_FAILURE;
		}
		
		unsigned long id = strtoul(argv[2], NULL, 10);
		if(id == 0)
		{
			fprintf(stderr, "Failed to parse id\n");
			return EXIT_FAILURE;
		}
		
		if(todo_delete(id) != 0)
		{
			fprintf(stderr, "Failed to delete todo!\n");
			return EXIT_FAILURE;
		}
		
		printf("Deleted todo!\n");
	}
	else
	{
			print_usage(argv[0]);
			return EXIT_FAILURE;
	}


	return EXIT_SUCCESS;
}
