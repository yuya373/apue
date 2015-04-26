#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char const* argv[])
{
    if (argc != 2) {
        fprintf(stderr, "usage a.out <pathname>\n");
    }
    // real user is login user
    // access check permission by real user
    if (access(argv[1], R_OK) < 0) {
        perror(strerror(errno));
    } else {
        printf("read access OK\n");
    }
    if (open(argv[1], O_RDONLY) < 0) {
        perror(strerror(errno));
    } else {
        printf("open for reading OK\n");
    }
    if (open(argv[1], O_RDWR) < 0) {
        perror(strerror(errno));
    } else {
        printf("open for read write OK\n");
    }
    return 0;
}
