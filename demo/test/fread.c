#include <stdio.h>
#include <assert.h>


int main()
{
    int ret = 0;
    unsigned long id = 10000;
    FILE *fp = NULL;
    char id_str[sizeof(unsigned long)];

    printf("%d ",sizeof(id_str));

    fp = fopen("./1.ini","rb");
    assert(NULL != fp);

    ret = fread(id_str,1,sizeof(id_str),fp);
    printf("%d %s\n",ret,id_str);

    fclose(fp);
}
