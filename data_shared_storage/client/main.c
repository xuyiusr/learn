#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SERVER_ADDR "127.0.0.1"
#define SERVER_PORT 4321

/* 初始化消息套接字 */
static int init_sock(int *sock)
{
    int fd = -1;

    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(fd == -1)
    {
        perror("create socket failed\n");
        return -1;
    }

    *sock = fd;
    return 0;
}

/* 发送消息 */
static int send_oper(int sock, char *buf)
{
    int ret = -1;
    struct sockaddr_in dest_addr = {0};

    dest_addr.sin_family = AF_INET;//使用IPv4协议
    dest_addr.sin_port = htons(SERVER_PORT);//设置接收方端口号
    dest_addr.sin_addr.s_addr = inet_addr(SERVER_ADDR); //设置接收方IP 

    ret = sendto(sock, buf, strlen(buf), 0, (struct sockaddr *)&dest_addr,sizeof(dest_addr));
    if (ret == -1){
        perror("send msg failed\n");
        return -1;
    }
    
    printf("send msg[%s] suc\n", buf);

    return 0;
}

/* 创建表 */
static int create_table(int sock)
{
    char buf[1024] = {0};

    snprintf(buf, sizeof(buf), "0;svnAccounts;jobNumber INT,user CHAR(50),pwd INT;;");

    send_oper(sock, buf);
}

/* 插入记录 */
static int insert_record_into_table(int sock)
{
    char buf[1024] = {0};

    snprintf(buf, sizeof(buf), "1;svnAccounts;jobNumber:1,user:\"xuyi\",pwd:123456;;");

    send_oper(sock, buf);
}

/* 更新记录 */
static int update_record_from_table(int sock)
{
    char buf[1024] = {0};

    snprintf(buf, sizeof(buf), "2;svnAccounts;pwd=654321;user=\"xuyi\";");

    send_oper(sock, buf);
}

int main(int argc, char * argv[])
{
    int ret = 0;
    int sock = -1;
    
    if (argc < 1)
        return -1;

    ret = init_sock(&sock);
    assert(!ret);

    switch(atoi(argv[1])){
        case 0:
            create_table(sock);
            break;
        case 1:
            insert_record_into_table(sock);
            break;
        case 2:
            update_record_from_table(sock);
            break;
        default:
            break;
    }

    close(sock);
    return 0;
}
