#include <netdb.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

// Determine if we should show the localized time as well
// Time must be in iso8601 format
bool shouldShowLocalTime(int argc, char *argv[])
{
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-l") == 0 || strcmp(argv[i], "--local") == 0)
        {
            return true;
        }
    }

    return false;
}

// Convert iso8601 UTC time to local time
bool do_localizeTime(const char *iso8601_time, char *output, size_t output_len)
{
    struct tm utc = {0};

    if (strptime(iso8601_time, "%Y-%m-%dT%H:%M:%S", &utc) == NULL)
    {
        fprintf(stderr, "Failed to parse timestamp\n");
        return false;
    }

    time_t utc_time = timegm(&utc);

    if (utc_time == (time_t)-1)
    {
        fprintf(stderr, "Failed to convert UTC time\n");
        return false;
    }

    struct tm local;

    if (localtime_r(&utc_time, &local) == NULL)
    {
        fprintf(stderr, "Failed to convert local time\n");
        return false;
    }

    if (strftime(output, output_len, "%Y-%m-%d %H:%M:%S %Z", &local) == 0)
    {
        fprintf(stderr, "Failed to format time string\n");
        return false;
    }

    return true;
}

// daytime client
int main(int argc, char *argv[])
{
    struct addrinfo hints, *result, *rp;
    int error, sfd;

    bool localizeTime = shouldShowLocalTime(argc, argv);

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
    char buffer[4096];
    size_t total_read = 0;

    while ((bytes_read = read(sfd, buffer + total_read, sizeof buffer - total_read - 1)) > 0)
    {
        total_read += (size_t)bytes_read;
        if(total_read == sizeof buffer -1)
        {
            break;
        }
    }

    if (bytes_read == -1)
    {
        perror("read");
        close(sfd);
        return EXIT_FAILURE;
    }

    close(sfd);
    buffer[total_read] = '\0';

    if (localizeTime)
    {
        char local_time[64];
        if(do_localizeTime(buffer, local_time, sizeof local_time))
            printf("Localized Time: %s\n", local_time);
    }

    printf("Server Response: %s\n", buffer);

    return EXIT_SUCCESS;
}
