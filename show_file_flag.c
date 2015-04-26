#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const* argv[])
{
    int val;

    if (argc != 2) {
        fprintf(stderr, "usage: a.out <descriptor#>\n");
    }

    if ((val = fcntl(atoi(argv[1]), F_GETFL, NULL)) < 0) {
        perror(strerror(errno));
    }
    switch (val & O_ACCMODE) {
        case O_RDONLY:
             printf("read only");
             break;
        case O_WRONLY:
             printf("write only");
             break;
        case O_RDWR:
             printf("read write");
             break;
        default:
             fprintf(stderr, "unknown access mode\n");
    }
    if (val & O_APPEND) {
        printf(", append");
    }
    if (val & O_NONBLOCK) {
        printf(", nonblocking");
    }
    if (val & O_SYNC) {
        printf(", synchronous writes");
    }
    putchar('\n');
    return 0;
}
