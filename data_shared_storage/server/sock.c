#include "sock.h"

/* 创建监听套接字 */
int init_socket(int *fd, int port)
{
    int sock = -1;
    struct sockaddr_in servaddr;

    assert(sock);

    if ((sock = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
        return -1;
    
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sock, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0){
        close(sock);
        return -1;
    }

    *fd = sock;
    return 0;
}

/* 接收消息 */
int socket_recv(int fd, char *buf, int buf_len)
{
    struct sockaddr_in  src_addr = {0};  //用来存放对方(信息的发送方)的IP地址信息
    int len = sizeof(src_addr); //地址信息的大小
    int ret = 0;

    ret = recvfrom(fd, buf, buf_len, 0, (struct sockaddr *)&src_addr, &len);
    if (ret == -1)
        return -1;

    return 0;
}
