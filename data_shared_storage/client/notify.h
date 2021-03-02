#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef (int *callback)(void *);

typedef struct notify_param{
    char table[128];
    callback func;
}

