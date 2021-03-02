#include <stdio.h>
#include <stdlib.h>

typedef struct my_test_t
{
    int a ;
    char b;
}my_test;


int main()
{
    my_test a;
    my_test b;
    printf("%d\n",sizeof(a));
    printf("%d\n",memcmp(&a,&b,sizeof(a)));
}
