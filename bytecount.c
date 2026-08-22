#include <stdio.h>
#include <stdbool.h>

bool verifyArguments(int argc, char *argv[])
{
    if(argc != 2)
    {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return false;
    }

    return true;
}

int main(int argc, char *argv[])
{
    if(!verifyArguments(argc, argv))
    {
        return 1;
    }

    FILE* fptr;
    unsigned char buffer[4096];
    size_t bytes_read;
    size_t total = 0;

    fptr = fopen(argv[1], "r");
    if(fptr == NULL)
    {
        perror("Error opening file");
        return 1;
    }

    while((bytes_read = fread(buffer, 1, sizeof buffer, fptr)) > 0)
    {
        total += bytes_read;
    }

    if(ferror(fptr))
    {
        perror("Error reading file");
        fclose(fptr);
        return 1;
    }

    fclose(fptr);

    printf("Bytes read: %zu\n", total);

    return 0;
}
