#include <dirent.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

typedef int Myfunc(const char *, const struct stat *, int);

static Myfunc myfunc;
static int myftw(const char *, Myfunc * );
static int dopath(Myfunc *);
static long nreg, ndir, nblk, nchr, nfifo, nslink, nsock, ntot;
static char* path_alloc(size_t *);

int main(int argc, char const* argv[])
{
    int ret;

    if (argc != 2) {
        fprintf(stderr, "usage: a.out <starting-pathname>\n");
        exit(EXIT_FAILURE);
    }

    ret = myftw(argv[1], myfunc);

    ntot = nreg + ndir + nblk + nchr + nfifo + nslink + nsock;

    if (ntot == 0) {
        ntot = 1;
    }

    printf("regular files  = %7ld, %5.2f %%\n", nreg, (nreg * 100.0) / ntot);
    printf("directories    = %7ld, %5.2f %%\n", ndir, (ndir * 100.0) / ntot);
    printf("block special  = %7ld, %5.2f %%\n", nblk, (nblk * 100.0) / ntot);
    printf("char special   = %7ld, %5.2f %%\n", nchr, (nchr * 100.0) / ntot);
    printf("FIFOs          = %7ld, %5.2f %%\n", nfifo, (nfifo * 100.0) / ntot);
    printf("symbolic links = %7ld, %5.2f %%\n", nslink, (nslink * 100.0) / ntot);
    printf("sockets        = %7ld, %5.2f %%\n", nsock, (nsock * 100.0) / ntot);
    exit(ret);
}

#define FTW_F 1
#define FTW_D 2
#define FTW_DNR 3
#define FTW_NS 4

static char* fullpath;
static size_t pathlen;

static int myftw(const char* pathname, Myfunc* func)
{
    fullpath = path_alloc(&pathlen);
    if (pathlen <= strlen(pathname)) {
        pathlen = strlen(pathname) * 2;
        if ((fullpath = realloc(fullpath, pathlen)) == NULL) {
            perror(strerror(errno));
        }
    }
    strcpy(fullpath, pathname);
    printf("searching dir is: %s\n", fullpath);
    return(dopath(func));
}

static int dopath(Myfunc* func)
{
    struct stat statbuf;
    struct dirent* dirp;
    DIR* dp;
    int ret, n;

    if (lstat(fullpath, &statbuf) < 0) {
        return(func(fullpath, &statbuf, FTW_NS));
    }
    if (S_ISDIR(statbuf.st_mode) == 0) {
        return(func(fullpath, &statbuf, FTW_F));
    }

    if ((ret = func(fullpath, &statbuf, FTW_D)) != 0) {
        return(ret);
    }
    n = strlen(fullpath);
    if (n + NAME_MAX + 2 > pathlen) {
        pathlen *= 2;
        if ((fullpath = realloc(fullpath, pathlen)) == NULL) {
            perror(strerror(errno));
        }
    }
    fullpath[n++] = '/';
    fullpath[n] = 0;
    if ((dp = opendir(fullpath)) == NULL) {
        perror(strerror(errno));
        return(func(fullpath, &statbuf, FTW_DNR));
    }
    while ((dirp = readdir(dp)) != NULL) {
        if (strcmp(dirp->d_name, ".") == 0 || strcmp(dirp->d_name, "..") == 0) {
            continue;
        }
        strcpy(&fullpath[n], dirp->d_name);
        if ((ret = dopath(func)) != 0) {
            break;
        }
    }
    fullpath[n-1] = 0;
    if (closedir(dp) < 0) {
        perror(strerror(errno));
    }
    return(ret);
}

static int myfunc(const char* pathname, const struct stat* statptr, int type)
{
    switch (type) {
        case FTW_F:
            switch (statptr->st_mode & S_IFMT) {
                case S_IFREG:
                    nreg++;
                    break;
                case S_IFBLK:
                    nblk++;
                    break;
                case S_IFCHR:
                    nchr++;
                    break;
                case S_IFIFO:
                    nfifo++;
                    break;
                case S_IFLNK:
                    nslink++;
                    break;
                case S_IFSOCK:
                    nsock++;
                    break;
                case S_IFDIR:
                    fprintf(stderr, "for S_DIR for %s\n", pathname);
                    exit(EXIT_FAILURE);
            }
            break;
        case FTW_D:
            ndir++;
            break;
        case FTW_DNR:
            fprintf(stderr, "can't read directory %s\n", pathname);
            exit(EXIT_SUCCESS);
        case FTW_NS:
            fprintf(stderr, "stat error for %s\n", pathname);
            exit(EXIT_FAILURE);
        default:
            fprintf(stderr, "unknown type %d for pathname %s\n", type, pathname);
    }
    return(0);
}

static char* path_alloc(size_t* size)
{
    return malloc(*size);
}
