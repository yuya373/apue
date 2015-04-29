#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char const* argv[])
{
    struct stat buf;
    if (stat("foo", &buf) < 0) {
        perror(strerror(errno));
    }
    if (chmod("foo", (buf.st_mode & ~S_IXGRP) | S_ISGID) < 0) {
        perror(strerror(errno));
    }
    if (chmod("bar", S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) < 0) {
        perror(strerror(errno));
    }
    return 0;
}
