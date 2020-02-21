#include "sock_client.h"

int main()
{
    int connfd = -1;
    unsigned char conn_ip[INET6_ADDRSTRLEN] = {0};
    int port = 0;
    char content[65535] = {0};
    int len = 0;

    printf("input ip to connect : ");
    scanf("%s",conn_ip);
    connfd = connect_ipv6_socket(conn_ip,10240);
    assert(-1 != connfd);

    while(1)
    {
        memset(content,0x0,sizeof(content));
        printf("# ");
        scanf("%s",content);
        
        len = send(connfd,content,strlen(content),0);
        if (-1 == len)
        {
            printf("send content failed.err:%s\n",strerror(errno));
            break;
        }
    }
    return EXIT_SUCCESS;
}
