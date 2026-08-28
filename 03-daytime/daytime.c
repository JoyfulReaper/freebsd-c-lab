#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    struct addrinfo hints, *result, *rp;
    int error, sfd;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    error = getaddrinfo("daytime.kgivler.com", "13", &hints, &result);
    if (error != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(error));
        return EXIT_FAILURE;
    }

    for (rp = result; rp != NULL; rp = rp->ai_next)
    {
        sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (sfd == -1)
            continue;

        if (connect(sfd, rp->ai_addr, rp->ai_addrlen) != -1)
        {
            printf("Connected using: %s\n", rp->ai_family == AF_INET ? "IPv4" : "IPv6");
            break;
        }

        close(sfd);
    }

    if (rp == NULL)
    {
        fprintf(stderr, "failed to connect\n");
        freeaddrinfo(result);
        return EXIT_FAILURE;
    }

    freeaddrinfo(result);

    ssize_t bytes_read = 0;
    unsigned char buffer[4096];
    while ((bytes_read = read(sfd, buffer, sizeof buffer)) > 0)
    {
        write(STDOUT_FILENO, buffer, bytes_read);
    }

    if (bytes_read == -1)
    {
        perror("read");
        close(sfd);
        return EXIT_FAILURE;
    }

    close(sfd);
    return EXIT_SUCCESS;
}
