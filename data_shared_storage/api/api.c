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
#include <pthread.h>

enum {
    ADD_TABLE = 0,
    INSERT_RECORD = 1,
    UPDATE_RECORD = 2,
    DELETE_RECORD = 3,
    SELECT_RECORD = 4,
    REG_ENCODE = 5,
    UNREG_RNCODE = 6,
    REG_NOTIFY = 7,
    UNREG_NOTIFY = 8,
    ALL_BUSINESS_CODE,
}

#define SERVER_ADDR "127.0.0.1"
#define SERVER_PORT 4321
static __thread sock = -1;
static __thread init = 0;

/* 初始化消息套接字 */
static int init_sock()
{
    int fd = -1;

    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(fd == -1)
    {
        perror("create socket failed\n");
        return -1;
    }

    sock = fd;
    init = 1;
    return 0;
}

/* 发送消息 */
static int send_oper(char *buf)
{
    int ret = -1;
    struct sockaddr_in dest_addr = {0};

    if (!init)
        init_sock();

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

/* 创建表
 * @table : 创建的表名
 * @field : 创建的字段信息，字段名+空格+属性,多个字段用，分隔
 */
int make_create_table_req(char *table, char *field)
{
    char buf[1024] = {0};

    //snprintf(buf, sizeof(buf), "0;svnAccounts;jobNumber INT,user CHAR(50),pwd INT;;");
    snprintf(buf, sizeof(buf), "%d;%s;%s;;", ADD_TABLE, table, field);

    send_oper(sock, buf);
}

/* 插入记录
 * @table : 插入的表名
 * @value : 插入的值, 字段名+:+值，多个字段用，分隔
 */
int make_insert_record_req(char *table, char *value)
{
    char buf[1024] = {0};

    //snprintf(buf, sizeof(buf), "1;svnAccounts;jobNumber:1,user:\"xuyi\",pwd:123456;;");
    snprintf(buf, sizeof(buf), "%d;%s;%s;;", INSERT_RECORD, table, value);

    send_oper(sock, buf);
}

/* 更新记录
 * @table : 更新的表
 * @modify : 需要修改的表
 * @condition : 更新条件
 */
int make_update_record_req(char *table, char *modify, char *condition)
{
    char buf[1024] = {0};

    //snprintf(buf, sizeof(buf), "2;svnAccounts;pwd=654321;user=\"xuyi\";");
    snprintf(buf, sizeof(buf), "%d;%s;%s;%s;", UPDATE_RECORD, table, modify, condition);

    send_oper(sock, buf);
}

/* 添加字段加密
 */
int make_reg_column_ciphertext_req(char *table, char *column)
{
    char buf[1024] = {0};

    snprintf(buf, sizeof(buf), "%d;%s;%s;", REG_ENCODE, table, column);

    send_oper(sock, buf);
}

/* 注册表监听
 */
int make_reg_listen_table_notify_req(char *table)
{
    char buf[1024] = {0};

    snprintf(buf, sizeof(buf), "%d;%s;", REG_NOTIFY, table);

    send_oper(sock, buf);
}

