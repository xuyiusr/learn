#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include "log.h"

#define CYCLE_SIZE 1024
#define DOMAIN_SIZE 256

enum{
    DOMAIN_PARSER = 2,
    DOMAIN_INPUT,
    TOTAL_THREAD_NUM,
};

typedef struct cycle{
    int in;
    int out;
    int size;
    char *data;
}cycle;

// 数据交互
static cycle s_domain_list[DOMAIN_PARSER];

static int exec_cmd(const char *cmd, char *res, const int res_len)
{
    FILE *fd = NULL;
    int read_len = 0;
    char tmp[1024];
    
    fd = popen(cmd, "r");
    if (!fd)
        return -1;
    
    while (fgets(tmp, sizeof(tmp), fd) != NULL){
        if (tmp[strlen(tmp) - 1] == '\n') {
            tmp[strlen(tmp) - 1] = '\0';
        }

        read_len += strlen(tmp);
        if (read_len >= res_len){
            pclose(fd);
            return -2;
        }
        
        strcat(res, tmp);
    }
    
    pclose(fd);
    return 0;
}

// 字符串hash
static unsigned int hash_func(char *buf)
{
    unsigned int hash = rand();
    int len = strlen(buf);

    while (len--)
        hash = ((hash << 5) + hash) +(tolower(*buf++)); /* hash * 33 + c */
    return hash;
}

// 初始化缓冲区
static int cycle_init(cycle *buf, int size)
{
    buf->data = (char *)malloc(size);
    if (!buf->data){
        printf("malloc buf failed\n");
        return -1;
    }

    buf->in = 0;
    buf->out = 0;
    buf->size = size;

    return 0;
}

// 读取缓冲区
static int cycle_get(cycle *buf, void *read_buf, int size)
{
    int tail_len = buf->size - 1 - buf->out;

    // 无可用数据
    if (buf->in == buf->out)
        return -1;

    // 读取数据
    // 缓冲区尾部长度不足
    if (tail_len < size){
        memcpy(read_buf, buf->data + buf->out, tail_len);
        memcpy(read_buf + tail_len, buf->data, size - tail_len);
        buf->out = size - tail_len;
    }
    else{
        memcpy(read_buf, buf->data + buf->out, size);
        buf->out += size;
    }

    return 0;
}

// 写入缓冲区
static int cycle_put(cycle *buf, void *set_buf, int size)
{
    int tail_len = buf->size - 1 - buf->in;

    // 写入数据超过可用
    if ((buf->size + buf->out - buf->in) < size){
        printf("buf free can not inser cur data\n");
        return -1;
    }

    // 写入数据
    // 缓冲区尾部长度不足
    if (tail_len < size){
        memcpy(buf->data + buf->in, set_buf, tail_len);
        memcpy(buf->data, set_buf + (size - tail_len), size - tail_len);
        buf->in = size - tail_len;
    }
    else{
        memcpy(buf->data + buf->in, set_buf, size);
        buf->in += size;
    }

    return 0;
}

static void dump_cycle(int i){
    cycle *buf = &s_domain_list[i];

    printf("dump_cycle %d: in %d\tout %d\tsize %d\n", 
            i, buf->in, buf->out, buf->size);
}

// 插入域名列表
static int insert_domain_list(int i, const char *domain)
{
    cycle *buf = &s_domain_list[i];
    int str_len = strlen(domain);

    if (cycle_put(buf, (void *)&str_len, sizeof(int)))
        return -1;
    
    if (cycle_put(buf, (void *)domain, str_len))
        return -1;
    
    return 0;
}

// 获取待解析域名列表
static int get_domain_list(int i, char *domain)
{
    cycle *buf = &s_domain_list[i];
    int read_len;

    if (cycle_get(buf, &read_len, sizeof(int)))
        return -1;
    printf("get len %d\n", read_len);

    if (cycle_get(buf, domain, read_len))
        return -1;
    printf("get buf %s\n", domain);

    return 0;
}

// 站长之家
static int parser_domain_by_icp(const char *domain)
{
    char cmd[1024] = {0};
    char res[1024] = {0};
    
    snprintf(cmd, sizeof(cmd), "curl -s http://icp.chinaz.com/%s | grep company "
            "| awk -F \"company/\" '{ print $2 }' | awk -F \"\\\"\" '{print $1}'", domain);

    printf("cmd:%s\n", cmd);
    if (!exec_cmd(cmd, res, sizeof(res)) && strlen(res)){
        printf("\e[31m%s -> %s\n\e[0m", domain, res);
        return 0;
    }
    else{
        printf("icp select failed.\n");
        return -1;
    }
}

// 备案巴巴
static int parser_domain_by_beian88(const char *domain)
{
    char cmd[1024] = {0};
    char res[1024] = {0};
    
    snprintf(cmd, sizeof(cmd), "curl -s https://www.beian88.com/%s | grep m_ba_Name "
            "| awk -F \"m_ba_Name\\\">\" '{print $2}' | awk -F \"<\" '{print $1}'", domain);

    printf("cmd:%s\n", cmd);
    if (!exec_cmd(cmd, res, sizeof(res)) && strlen(res)){
        printf("\e[31m%s -> %s\n\e[0m", domain, res);
        return 0;
    }
    else{
        printf("beian88 select failed.\n");
        return -1;
    }
} 

// 备案备案
static int parser_domain_by_beianbeian(const char *domain)
{
    char cmd[1024] = {0};
    char res[1024] = {0};
    
    snprintf(cmd, sizeof(cmd), "curl -s http://www.beianbeian.com/search/%s.html | grep -A 12 单位名称 "
            "| tail -1 | grep -oP '(?<=<td>).*(?=</td>)'", domain);

    printf("cmd:%s\n", cmd);
    if (!exec_cmd(cmd, res, sizeof(res)) && strlen(res)){
        printf("\e[31m%s -> %s\n\e[0m", domain, res);
        return 0;
    }
    else{
        printf("beianbeian select failed.\n");
        return -1;
    }
} 

// 域名解析
static void domain_parser(void *param)
{
    int list_num = (int)param;
    char domain[DOMAIN_SIZE];
    char cmd[1024];

    while(1){
        memset(domain, 0x0, sizeof(domain));
        if (!get_domain_list(list_num, domain)){
            printf("%d get domain %s\n", list_num, domain);
            
            if (!parser_domain_by_icp(domain)){
                continue;
            }
            if (!parser_domain_by_beian88(domain)){
                continue;
            }
            if (!parser_domain_by_beianbeian(domain)){
                continue;
            }
            printf("\e[31m%s select failed.\n\e[0m", domain);
        }
        usleep(50000);
    }

}

// 输入线程
static void domain_input(void *param)
{
    int i = 0;
    char domain[1024];
    char *input_file = (char *)param;
    FILE *fd = NULL;

    fd = fopen(input_file,"r");
    if (!fd){
        printf("can not open file %s, err: %s\n", input_file, strerror(errno));
    }

    while(fgets(domain, sizeof(domain), fd) != NULL ){
        // 获取输入
        //snprintf(domain, "%s","www.sina.com");
        if (domain[strlen(domain) - 1] == '\n')
            domain[strlen(domain) - 1] = '\0';
        //printf("get a domain %s\n", domain);    
        // 计算hash
        i = hash_func(domain) % DOMAIN_PARSER;
        // 负载均衡分配
        if (insert_domain_list(i, domain))
            printf("insert list failed.\n");
        usleep(50000);
    }

    fclose(fd);
}

// 工作线程监控
static void thread_main(pthread_t *work_id)
{
    int i = 0;

    while(1){
        for (i = 0; i < TOTAL_THREAD_NUM; i++){
            if (pthread_kill(work_id[i],0) == ESRCH){
                if (i <= DOMAIN_PARSER){
                    pthread_create(&work_id[i], NULL, (void *)domain_parser, (void *)i);
                }
            }
        }

        usleep(50000);
    }
}

int main(int argc, char *argv[])
{
    int i = 0;
    int ret = 0;
    char file[256];
    pthread_t work_id[TOTAL_THREAD_NUM];

    if (argc == 2){
        snprintf(file, sizeof(file), "%s", argv[1]);
    }
    else{
        printf("./domain_parser [input_file]\n");
        return 1;
    }

    for (i = 0; i < TOTAL_THREAD_NUM; i++){
        printf("start thread %d , total %d\n", i, TOTAL_THREAD_NUM - 1);

        // 启动解析线程
        if (i <= DOMAIN_PARSER){
            if (cycle_init(&s_domain_list[i], CYCLE_SIZE)){
                printf("init cycle failed\n");
                return 1;
            }
            pthread_create(&work_id[i], NULL, (void *)domain_parser, (void *)i);
        }

        // 启动输入线程
        if (i == DOMAIN_INPUT){
            pthread_create(&work_id[i], NULL, (void *)domain_input, (void *)file);
        }
    }

    thread_main(work_id);

    return 0;
}
