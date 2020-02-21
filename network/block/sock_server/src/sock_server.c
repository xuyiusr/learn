#include "sock_server.h"

int main()
{
    int listenfd = -1;
    int connfd = -1;
    int len = 0;
    char content[65535] = {0};

    listenfd = tcp_server_socket_ipv6(10240);
    assert(-1 != listenfd);

    connfd = tcp_accept_ipv6(listenfd);
    assert(-1 != connfd);

    while(1)
    {
        usleep(2000);
        memset(content,0x0,sizeof(content));
        len = recv(connfd,content,sizeof(content),0);
        if (0 == len)
        {
            continue;
        }
        else if (-1 == len)
        {
            printf("read content error.err:%s\n",strerror(errno));
            break;
        }
        else
        {
            printf("read suc,content:%s\n",content);
        }
    }
    return EXIT_SUCCESS;
}
