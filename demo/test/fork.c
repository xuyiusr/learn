#include<stdio.h>

int main()
{
    int pid = 0;

    pid = fork();

    switch(pid)
    {
            case 0:
               break; 
            case -1:
               break;
            default:
               break;
    }
    
    return 0;
}
