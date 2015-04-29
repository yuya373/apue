#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char const* argv[])
{
    if (open("tempfile", O_RDWR) < 0) {
        perror(strerror(errno));
    }
    if (unlink("tempfile") < 0) {
        perror(strerror(errno));
    }
    printf("file unlinked\n");
    sleep(15);
    printf("done\n");
    return 0;
}
