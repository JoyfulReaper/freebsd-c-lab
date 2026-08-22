#include <stdio.h>
#include <stdbool.h>

bool verifyArguments(int argc, char *argv[])
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
    if(!verifyArguments(argc, argv))
    {
        return 1;
    }

    FILE* src;
    FILE* dest;
    unsigned char buffer[4096];
    size_t bytes_read;

    src = fopen(argv[1], "rb");
    dest = fopen(argv[2], "wb");

    if(src == NULL)
    {
        perror("Error opening source file");
        return 1;
    }

    if (dest == NULL)
    {
        perror("Error opening dest file");
        fclose(src);
        return 1;
    }

    while((bytes_read = fread(buffer, 1, sizeof buffer, src)) > 0)
    {
        fwrite(buffer,1,bytes_read, dest);
        if(ferror(dest))
        {
            perror("Error writing to dest");
            fclose(src);
            fclose(dest);
            return 1;
        }
    }

    if(ferror(src))
    {
        perror("Error reading src");
        fclose(src);
        fclose(dest);

        return 1;
    }

    fclose(src);
    fclose(dest);

    printf("File copied!\n");

    return 0;
}
