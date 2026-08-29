#include <stdio.h>
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
