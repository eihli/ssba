#include <assert.h>
#include <stdio.h>
#include <dirent.h>

int main(int argc, char *argv[])
{
    DIR *dp = opendir(".");
    assert(dp != NULL);
    struct dirent *d;
    while ((d = readdir(dp)) != NULL) {
        printf("%lu %lx %s\n", d->d_ino, d->d_off, d->d_name);
    }
    closedir(dp);
    return 0;
}
