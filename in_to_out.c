#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#define BUFFSIZE 4096

int main(int argc, char const* argv[])
{
    int n;
    char buf[BUFFSIZE];

    while ((n = read(STDIN_FILENO, buf, BUFFSIZE)) > 0) {
        if (write(STDOUT_FILENO, buf, n) != n) {
            perror(strerror(errno));
        }
    }
    if (n < 0) {
        perror(strerror(errno));
    }
    return 0;
}
