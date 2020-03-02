#include <stdio.h>  
#include <errno.h>  
#include <unistd.h>  
#include <sys/types.h>  
#include <sys/stat.h> 
#include <ftw.h>

long long int total_dir_size;

int sum_dir(const char *fpath, const struct stat *sb, int typeflag)
{
    total_dir_size += sb->st_size;
    return 0;
}

int main()
{
    char path[256];

    scanf("%s",path);
    ftw(path,&sum_dir,1);

    printf("size %u\n",total_dir_size);
}
