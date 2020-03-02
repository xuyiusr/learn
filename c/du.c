#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>

int get_dir_size(char *dir_path, int64_t *dir_size)
{
    DIR *dir;
    struct dirent *dirp;
    char file_abs_path[256];
    struct stat buf;
    int64_t total_size_byte = 0;

    dir = opendir(dir_path);
    if(NULL == dir)
    {
        return 1;
    }

    while(NULL != (dirp = readdir(dir)))
    {
        /* dir type, jump */
        if(DT_DIR == dirp->d_type)
        {
            printf("dir %s jump\n",dirp->d_name);
            continue;
        }

        /* file type */
        snprintf(file_abs_path,sizeof(file_abs_path),"%s/%s",dir_path,dirp->d_name);
        if(stat(file_abs_path,&buf))
        {
            printf("%s stat error.\n",file_abs_path);
            continue;
        }
        printf("%s file size %d\n",file_abs_path,buf.st_size);
        total_size_byte += buf.st_size;
    }

    *dir_size = total_size_byte;
    return 0;
}
        
int main()
{
    int ret = 0;
    char path[256];
    int64_t dir_size;

    printf("input path:\n> ");
    scanf("%s",path);

    ret = get_dir_size(path,&dir_size);

    printf("dir_size %lld\n",dir_size);

    return 0;
}
