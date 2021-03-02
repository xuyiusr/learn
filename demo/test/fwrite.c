#include <stdio.h>
#include <assert.h>


int main()
{
    int ret = 0;
    unsigned long id = 10000;
    FILE *fp = NULL;
    char id_str[20];

    fp = fopen("./1.ini","wb");
    assert(NULL != fp);

    snprintf(id_str,sizeof(id_str),"%ld",id);
    ret = fwrite(id_str,1,sizeof(unsigned long),fp);
    printf("%d",ret);

    fclose(fp);
}
