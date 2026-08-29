#ifndef TODO_H
#define TODO_H

#include <stdbool.h>

struct todo {
	unsigned long id;
	char *title;
	char *description;
	bool completed;
};


int todo_add(struct todo *todo);
int todo_list(void);
int todo_complete(unsigned long id, bool completed);
int todo_delete(unsigned long id);

#endif
