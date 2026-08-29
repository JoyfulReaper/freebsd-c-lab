#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "todo.h"

#define TODO_FILE "todos.txt"
#define TODO_TEMP_FILE "todos.tmp"

static unsigned long todo_next_id(void);
static int todo_parse(char *line, struct todo *todo);

int todo_add(const struct todo *todo)
{
	FILE *file = fopen(TODO_FILE, "a");
	if (file == NULL)
	{
		perror("fopen");
		return -1;
	}
	
	unsigned long id = todo_next_id();
	if(id == 0)
		return -1;
	
	if(fprintf(
		file,
		"%lu|%d|%s|%s\n",
		id,
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
		struct todo todo = {0};
		if(todo_parse(line, &todo) != 0)
		{
			fprintf(stderr, "Failed to parse record: %s\n", line);
		}
		
		printf("Id: %lu Title: %s Desc: %s Completed: %s\n",
			todo.id, todo.title, todo.description, todo.completed ? "true" : "false");
	}
	
	if(fclose(file) != 0)
	{
		perror("fclose");
		return -1;
	}
	
	return 0;
}

int todo_complete(unsigned long id)
{
	return 0;
}

static int todo_parse(char *line, struct todo *todo)
{
		line[strcspn(line, "\n")] = '\0';
		char *tokens[4];
		
		tokens[0] = strtok(line, "|");
		for(int i = 1; i < 4; i++)
		{
			tokens[i] = strtok(NULL, "|");
		}
		
		for(int i = 0; i<4; i++)
		{
			if(tokens[i] == NULL)
				return -1;
		}
		
		todo->id = strtoul(tokens[0], NULL, 10);
		todo->title = tokens[2];
		todo->description = tokens[3];
		todo->completed = strtoul(tokens[1], NULL, 10) != 0;
		
		return 0;
}

static unsigned long todo_next_id(void)
{
	unsigned long max_id = 0;
	
	struct stat buffer;
	if(stat(TODO_FILE, &buffer) != 0)
	{
		return 1;
	}
	
	FILE *file = fopen(TODO_FILE, "r");
	if(file == NULL)
	{
		perror("fopen");
		return 0;
	}
	
	char line[1024];
	while(fgets(line, sizeof line, file) != NULL)
	{
		char *token = strtok(line, "|");
		unsigned long current_id = strtoul(token, NULL, 10);
		if(current_id > max_id)
			max_id = current_id;
	}
	
	if(fclose(file) != 0)
	{
		perror("fclose");
		return 0;
	}
	
	return max_id + 1;
}
