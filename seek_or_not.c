#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

int main(int argc, char const* argv[])
{
    if (lseek(STDIN_FILENO, 0, SEEK_CUR) == -1) {
        perror(strerror(errno));
        printf("cannot seek\n" );
    } else {
        printf("seek OK\n");
    }
    return 0;
}
