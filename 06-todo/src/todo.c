#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include "todo.h"

#define TODO_FILE "todos.txt"
#define TODO_TEMP_FILE "todos.tmp"

enum todo_operation {
    TODO_DELETE,
    TODO_SET_COMPLETED
};

static unsigned long todo_next_id(void);
static int todo_parse(char *line, struct todo *todo);
static int todo_write(FILE *file, const struct todo *todo);
static int todo_rewrite(unsigned long id, enum todo_operation operation, bool completed);

int todo_add(struct todo *todo)
{
	unsigned long id = todo_next_id();
	if(id == 0)
		return -1;
	else
		todo->id = id;
	
	FILE *file = fopen(TODO_FILE, "a");
	if (file == NULL)
	{
		perror("fopen");
		return -1;
	}
	
	if(todo_write(file, todo) != 0)
	{
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
			continue;
		}
		
		printf("Id: %lu Title: %s Desc: %s Completed: %s\n",
			todo.id, todo.title, todo.description, todo.completed ? "true" : "false");
	}
	
	if (ferror(file))
	{
		perror("fgets");
		fclose(file);
		return -1;
	}
	
	if(fclose(file) != 0)
	{
		perror("fclose");
		return -1;
	}
	
	return 0;
}

static int todo_rewrite(unsigned long id, enum todo_operation operation, bool completed)
{
	FILE *file = fopen(TODO_FILE, "r");
	if(file == NULL)
	{
		perror("fopen");
		return -1;
	}
	
	FILE *tmp = fopen(TODO_TEMP_FILE, "w");
	if(tmp == NULL)
	{
		perror("fopen");
		fclose(file);
		return -1;
	}

	bool found = false;
	
	char line[1024];
	while (fgets(line, sizeof line, file) != NULL)
	{
		struct todo todo = {0};
		if(todo_parse(line, &todo) != 0)
		{
			fprintf(stderr, "Failed to parse record: %s\n", line);
			fclose(file);
			fclose(tmp);
			remove(TODO_TEMP_FILE);
			return -1;
		}
		
		if(todo.id == id)
		{
			found = true;
			
			if(operation == TODO_DELETE)
				continue;
				
			if(operation == TODO_SET_COMPLETED)
			{
				todo.completed = completed;
			}
		}
		
		if(todo_write(tmp, &todo) != 0)
		{
			fprintf(stderr, "Failed to write record: %lu", id);
			fclose(file);
			fclose(tmp);
			remove(TODO_TEMP_FILE);
			return -1;
		}
	}
	
	if (ferror(file))
	{
		perror("fgets");
		fclose(file);
		fclose(tmp);
		remove(TODO_TEMP_FILE);
		return -1;
	}
	
	if(fclose(file) != 0)
	{
		perror("fclose");
		fclose(tmp);
		remove(TODO_TEMP_FILE);
		return -1;
	}
	
	if(fclose(tmp) != 0)
	{
		perror("fclose");
		remove(TODO_TEMP_FILE);
		return -1;
	}
	
	if(found)
	{
		if(rename(TODO_TEMP_FILE, TODO_FILE) != 0)
		{
			perror("rename");
			remove(TODO_TEMP_FILE);
			return -1;
		}
	} else {
		remove(TODO_TEMP_FILE);
		return -1;
	}
	
	return 0;
}

int todo_delete(unsigned long id)
{
	return todo_rewrite(id, TODO_DELETE, false);
}

int todo_complete(unsigned long id, bool completed)
{
	return todo_rewrite(id, TODO_SET_COMPLETED, completed);
}


static int todo_write(FILE *file, const struct todo *todo)
{
	if(fprintf(
		file,
		"%lu|%d|%s|%s\n",
		todo->id,
		todo->completed ? 1 : 0,
		todo->title,
		todo->description) < 0) 
		{
			perror("fprintf");
			return -1;
		}
		
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
		if(errno == ENOENT)
			return 1;
			
		perror("stat");
		return 0;
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
		if(token == NULL)
		{
			fprintf(stderr, "Failed to parse line: %s", line);
			continue;
		}
		
		unsigned long current_id = strtoul(token, NULL, 10);
		if(current_id > max_id)
			max_id = current_id;
	}
	
	if (ferror(file))
	{
		perror("fgets");
		fclose(file);
		return 0;
	}
	
	if(fclose(file) != 0)
	{
		perror("fclose");
		return 0;
	}
	
	return max_id + 1;
}
