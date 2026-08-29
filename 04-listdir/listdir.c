#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
	char cwd[1024];
	DIR *dir;
	
	if(getcwd(cwd, sizeof cwd) != NULL)
	{
		dir = opendir(cwd);
		if(dir == NULL)
		{
			perror("opendir()");
			return EXIT_FAILURE;
		}
	} else {
		perror("getcwd()");
		return EXIT_FAILURE;
	}
	
	struct dirent *entry;
	
	while((entry = readdir(dir)) != NULL)
	{
		printf("%s\n", entry->d_name);
	}
	
	closedir(dir);
	return EXIT_SUCCESS;
}
