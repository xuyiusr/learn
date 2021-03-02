#include "notify.h"

#define NOTIFY_ADDR 127.0.0.1
#define NOTIFY_PORT 4322

static int notify_fd = -1;

/* 初始化通知句柄 */
int init_notify_fd()
{
    sockaddr_in addr_in;

    notify_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    addr_in.sin_family = AF_INET;
    addr_in.sin_port = htons(NOTIFY_PORT);
    addr_in.sin_addr.S_un.S_addr = htonl(NOTIFY_ADDR);

    if (bind(notify_fd, (sockaddr*)&addr_in, sizeof(sockaddr)) == SOCKET_ERROR)
    {
        printf("bind failed\n");
        return -1;
    }

    return 0;
}

