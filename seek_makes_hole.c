#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define FILE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

char buf1[] = "abcdefghij";
char buf2[] = "ABCDEFGHIJ";

int main(int argc, char const* argv[])
{
    int fd;
    if ((fd = creat("file.hole", FILE_MODE)) < 0) {
        perror(strerror(errno));
    }
    if (write(fd, buf1, 10) != 10) {
        perror(strerror(errno));
    }
    if (lseek(fd, 16384, SEEK_SET) == -1) {
        perror(strerror(errno));
    }
    if (write(fd, buf2, 10) != 10) {
        perror(strerror(errno));
    }

    if ((fd = creat("file.nohole", FILE_MODE)) < 0) {
        perror(strerror(errno));
    }

    char buf3[16394];
    memset(buf3, 'a', 16394);

    if (write(fd, buf3, 16394) != 16394) {
        perror(strerror(errno));
    }
    return 0;
}

// $ od -c file.hole
// this command will show the file filed with 0.
