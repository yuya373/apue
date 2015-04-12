#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char const* argv[])
{
    printf("hello world from process ID %ld\n", (long)getpid());
    return 0;
}
