#include <stdio.h>
#include <pthread.h>
#include <assert.h>
#include "data_processing.h"

#define WORK_THREAD_NUM 3 // 工作线程数
#define LISTEN_PORT 4321

typedef struct cache{
    int read;
    int save;
    char data[64][1024];
}cache;

cache thread_cache[WORK_THREAD_NUM];
extern _fun_st data_processing[];
pthread_mutex_t work_mutex;

static unsigned int hash_func(char *buf)
{
    unsigned int hash = rand();
    int len = strlen(buf);

    while (len--)
        hash = ((hash << 5) + hash) +(tolower(*buf++)); /* hash * 33 + c */
    return hash;
}

static int read_cache(cache *cache_data, char *dst)
{
    if (cache_data->read == cache_data->save)
        return -1;

    memcpy(dst, cache_data->data[cache_data->read], 1024);
    cache_data->read ++;

    return 0;
}

static int save_cache(char *buf)
{
    unsigned int i = hash_func(buf);

    i = i % WORK_THREAD_NUM;

    memcpy(thread_cache[i].data[thread_cache[i].save], buf, strlen(buf));
    thread_cache[i].save++;

    printf("save buf %s\n", buf);
    return 0;
}

static void work_func(void *param)
{
    int i = 0;
    int type = 0;
    char read[1024];
    int thread_n;
    cache *data;
    int ret = 0;
    char type_str[36];
    oper_param oper_data;
    char *p = NULL;
    int availd = 0;

    thread_n = (int)param;
    data = &thread_cache[thread_n];
    printf("[thread_%d]\n", thread_n);

    while(1){
        /* 读取消息 */
        if (read_cache(data, read) != 0){
            usleep(5000);
            continue;   
        }

        p = read;
        /* 判断消息类型 */
        get_string_before_char(p, type_str, ";");
        type = atoi(type_str);
        printf("msg type %d\n", type);

        for (i = 0; i < strlen(data_processing); i++){
            if (type == data_processing[i].business_code){
                availd = 1;
                break;
            }
        }

        if (availd == 0){
            printf("msg type error\n");
            usleep(5000);
            continue;
        }

        p += (strlen(type_str) + 1);
        /* 处理消息 */
        if (data_processing[type].deal_msg_func){
            ret = data_processing[type].deal_msg_func(p, &oper_data);
            if (ret){
                printf("deal_msg failed.\n");
                usleep(5000);
                continue;
            }
        }

        /* 加密数据 */
        if (data_processing[type].encode_func){
            ret = data_processing[type].encode_func(&oper_data);
            if (ret){
                printf("encode failed\n");
                usleep(5000);
                continue;
            }
        }

        /* 数据库操作 */
        if (data_processing[type].database_oper_func){
            /* 加锁 */
            pthread_mutex_lock(&work_mutex);
            ret = data_processing[type].database_oper_func(&oper_data);
            if (ret){
                /* 解锁 */
                pthread_mutex_unlock(&work_mutex);
                printf("database_oper failed\n");
                usleep(5000);
                continue;
            }
            /* 解锁 */
            pthread_mutex_unlock(&work_mutex);
        }

        /* 数据解密 */
        if (data_processing[type].decode_func){
            ret = data_processing[type].decode_func(NULL, NULL);
            if (ret){
                printf("decode failed\n");
                usleep(5000);
                continue;
            }
        }
        printf("deal msg suc\n");
    }
}

/* 接收消息 */
static void recv_func(void *param)
{
    int fd = -1;
    char buf[1024];


    fd = (int)param;
    printf("listen fd: %d\n", fd);

    while(1){
        if (socket_recv(fd, buf, sizeof(buf)) != 0)
            continue;

        if (buf[strlen(buf) - 1] == '\n')
            buf[strlen(buf) - 1] = 0;

        printf("[%s] recv msg: %s\n", __func__, buf);
        save_cache(buf);
    } 
}

/* 初始化工作线程 */
static int init_work_process(pthread_t *work_id)
{
    int i = 0;
    
    assert(work_id);

    for(i = 0; i < WORK_THREAD_NUM; i++){
        printf("start work thread %d\n", i);
        pthread_create(&work_id[i], NULL, (void *)work_func, i);
    }

    return 0;
}

/* 初始化接收线程 */
static int init_recv_process(pthread_t *recv_id, int fd)
{
    assert(recv_id);

    pthread_create(recv_id, NULL, (void*)recv_func, fd);

    return 0;
}

static void main_loop()
{
    while(1){
        usleep(2000);
    }
}

int main()
{
    int ret = 0;
    int sock = -1;
    pthread_t work_id[WORK_THREAD_NUM];
    pthread_t recv_id;

    /* 初始化通信套接字 */
    ret = init_socket(&sock, LISTEN_PORT);
    assert(!ret);

    /* 初始化线程锁 */
    ret = pthread_mutex_init(&work_mutex, NULL);
    assert(!ret);

    /* 初始化接收线程 */
    ret = init_recv_process(&recv_id, sock);
    assert(!ret);

    /* 初始化处理线程 */
    ret = init_work_process(work_id);
    assert(!ret);

    main_loop();
}

