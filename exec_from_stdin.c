#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

#define MAXLINE 4096

int main(int argc, char const* argv[])
{
    char buf[MAXLINE];
    pid_t pid;
    int status;

    printf("parent pid: %ld\n", (long)getpid());
    printf("> ");
    while (fgets(buf, MAXLINE, stdin) != NULL) {
       if (buf[strlen(buf) - 1] == '\n') {
           buf[strlen(buf) - 1] = 0;
       }
       if ((pid = fork()) < 0) {
           perror(argv[0]);
           // fprintf(stderr, "fork(2) failed: %s\n", strerror(errno));
           exit(1);
       }
       if (pid == 0) {
           printf("child pid: %ld\n", (long)getpid());
           execlp(buf, buf, (char *)0);
           perror(argv[0]);
           // fprintf(stderr, "execlp(2) failed: %s\n", strerror(errno));
           exit(1);
       }
       if ((pid = waitpid(pid, &status, WNOHANG)) < 0) {
           fprintf(stderr, "waitpid(2) failed\n");
           exit(1);
       }
       printf("> ");
    }
    return 0;
}
