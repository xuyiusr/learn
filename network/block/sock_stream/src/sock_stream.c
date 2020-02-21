#include "sock_stream.h"

/**
 * create a ipv6 socket.
 */
int create_ipv6_socket()
{
    return socket(AF_INET6,SOCK_STREAM,0);
}

/**
 * create a ipv6 socket for listen .
 * if suc return socket fd , if not return -1 .
 * @port : listen port .
 */
int tcp_server_socket_ipv6(int port)
{
    int fd = -1;
    int ret = -1;
#if 0
    struct  sockaddr_in {
         short  int  sin_family;               /* Address family */
         unsigned  short  int  sin_port;       /* Port number */
         struct  in_addr  sin_addr;            /* Internet address */
         unsigned  char  sin_zero[8];          /* Same size as struct sockaddr */
    };
#endif 
    struct sockaddr_in6 server_addr;

    do
    {  
        fd = create_ipv6_socket();
        if (0 > fd)
        {
            printf("create socket failed.err:%s.\n",strerror(errno));
            break;
        }

        memset(&server_addr,0x0,sizeof(server_addr));
        server_addr.sin6_family=AF_INET6;
        server_addr.sin6_addr=in6addr_any;
        server_addr.sin6_port=htons(port);
    
        if (-1 == bind(fd,(struct sockaddr *)&server_addr,sizeof(server_addr)))
        {
            printf("bind socket failed.port:%d,err:%s.\n",port,strerror(errno));
            break;
        }

        if (-1 == listen(fd,SOMAXCONN))
        {
            printf("listen socket failed.err:%s.\n",strerror(errno));
            break;
        }

        ret = fd;
    }while(0);

    if (-1 == ret)
    {
        if (0 <= fd)
        {
            close(fd);
        }
    }

    return ret;
}

int udp_server_socket_ipv6()
{

}

/**
 * accept a connect fd from client .
 * if suc return the connect fd , if not return -1 .
 * @fd : listen fd .
 */
int tcp_accept_ipv6(int fd)
{
    int accept_fd = -1;
    socklen_t addr_len = sizeof(struct sockaddr_in);
    struct sockaddr_in client_addr;

    accept_fd = accept(fd,(struct sockaddr*)&client_addr,(socklen_t *)&addr_len);
    if (-1 == accept_fd)
    {
        printf("accept failed.err:%s\n",strerror(errno));
    }
    return accept_fd;
}

/**
 * create a fd for connect to a ipv6 server.
 * if suc return the connection sockfd , if not return -1 .
 * @ip : ip address to connect .
 * @port : port to connect .
 */
int connect_ipv6_socket(unsigned char *ip, int port)
{
    int ret = -1;
    int sockfd = 0;
    struct sockaddr_in6 server_addr;

    do
    {
        sockfd = create_ipv6_socket();
        if (0 > sockfd)
        {
            printf("create socket failed.err:%s.\n",strerror(errno));
            break;
        }

        if (1 != inet_pton(AF_INET6,(char *)ip,&server_addr.sin6_addr))
        {
            printf("inet_pton failed.ip:%s.err:%s.\n",ip,strerror(errno));
            break;
        }
        memset(&server_addr,0x0,sizeof(server_addr));
        server_addr.sin6_family=AF_INET6;
        server_addr.sin6_port=htons(port);

        if (-1 == connect(sockfd,(struct sockaddr *)&server_addr,sizeof(server_addr)))
        {
            printf("connect to server failed.err:%s.\n",strerror(errno));
            break;
        }

        ret = sockfd;
    }while(0);

    if (-1 == ret)
    {
        if (0 <= sockfd)
        {
            close(sockfd);
        } 
    }

    return ret;
}
