#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "todo.h"

#define TODO_FILE "todos.txt"

int todo_add(const struct todo *todo)
{
	FILE *file = fopen(TODO_FILE, "a");
	if (file == NULL)
	{
		perror("fopen");
		return -1;
	}
	
	if(fprintf(
		file,
		"%lu|%d|%s|%s\n",
		todo->id,
		todo->completed ? 1 : 0,
		todo->title,
		todo->description) < 0) {
			perror("fprintf");
			fclose(file);
			return -1;
		}
		
	if (fclose(file) != 0) {
		perror("fclose");
		return -1;
	}
	
	return 0;
}

int todo_list(void)
{
	FILE *file = fopen(TODO_FILE, "r");
	if(file == NULL)
	{
		perror("fopen");
		return -1;
	}
	
	char line[1024];
	while (fgets(line, sizeof line, file) != NULL)
	{
		line[strcspn(line, "\n")] = '\0';
		char *tokens[4];
		
		tokens[0] = strtok(line, "|");
		for(int i = 1; i < 4; i++)
		{
			tokens[i] = strtok(NULL, "|");
		}
		
		struct todo todo = {
			.id = strtoul(tokens[0], NULL, 10),
			.completed = strtoul(tokens[1], NULL, 10) != 0,
			.title = tokens[2],
			.description = tokens[3]
		};
		
		printf("Id: %lu Title: %s Desc: %s Completed: %s\n",
			todo.id, todo.title, todo.description, todo.completed ? "true" : "false");
	}
	
	return 0;
}
