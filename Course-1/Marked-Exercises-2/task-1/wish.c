#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

int main(int argc, char* argv[])
{
    DIR *directory = opendir(argv[1]);
    struct dirent *contents;
    struct stat dir_stat;
    while((contents = readdir(directory)) != NULL)
    {
        stat(contents->d_name, &dir_stat);
        printf("%zu %s\n", dir_stat.st_size, contents->d_name);
    }
    closedir(directory);
    return 0;
}
