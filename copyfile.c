#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <errno.h>

bool verify_arguments(int argc, char *argv[])
{
    if(argc != 3)
    {
        fprintf(stderr, "Usage: %s <source> <dest>\n", argv[0]);
        return false;
    }

    return true;
}

int main (int argc, char *argv[])
{
    if(!verify_arguments(argc, argv))
    {
        return EXIT_FAILURE;
    }

    FILE* src;
    FILE* dest;
    unsigned char buffer[4096];
    size_t bytes_read;

    src = fopen(argv[1], "rb");
    if(src == NULL)
    {
        perror("Error opening source file");
        return EXIT_FAILURE;
    }

    struct stat src_stat;
    struct stat dest_stat;

    if (fstat(fileno(src), &src_stat) == -1)
    {
        perror("fstat source");
        fclose(src);
        return EXIT_FAILURE;
    }

    if(stat(argv[2], &dest_stat) == 0)
    {
        if(src_stat.st_dev == dest_stat.st_dev &&
            src_stat.st_ino == dest_stat.st_ino)
        {
            fprintf(stderr, "Source and destination are the same file\n");
            fclose(src);
            return EXIT_FAILURE;
        }
    } 
    else if (errno != ENOENT)
    {
        perror("stat destination");
        fclose(src);
        return EXIT_FAILURE;
    }

    dest = fopen(argv[2], "wb");
    if (dest == NULL)
    {
        perror("Error opening dest file");
        fclose(src);
        return EXIT_FAILURE;
    }

    while((bytes_read = fread(buffer, 1, sizeof buffer, src)) > 0)
    {
        size_t bytes_written = fwrite(buffer,1,bytes_read, dest);

        if(bytes_written != bytes_read)
        {
            perror("Error writing to dest");
            fclose(src);
            fclose(dest);
            return EXIT_FAILURE;
        }
    }

    if(ferror(src))
    {
        perror("Error reading src");
        fclose(src);
        fclose(dest);

        return EXIT_FAILURE;
    }

    fclose(src);
    if (fclose(dest) == EOF)
    {
        perror("Error closing destination");
        return EXIT_FAILURE;
    }

    puts("File copied!");

    return EXIT_SUCCESS;
}
