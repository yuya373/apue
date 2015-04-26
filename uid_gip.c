#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char const* argv[])
{
    printf("uid: %d, gid: %d\n", getuid(), getgid());
    printf("open max: %ld\n", sysconf(_SC_OPEN_MAX));
    printf("posix version: %ld\n", sysconf(_SC_VERSION));
    printf("line max: %ld\n", sysconf(_SC_LINE_MAX));
    printf("clock tick: %ld\n", sysconf(_SC_CLK_TCK));
    return 0;
}
