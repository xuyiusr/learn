#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/file.h>
#include <netinet/in.h>
#include <linux/types.h>
#include <sys/types.h>
#include <linux/netfilter.h>
#include <libnetfilter_queue/libnetfilter_queue.h>

int exec_cmd(const char *cmd, char *res , int res_len)
{
    FILE *fp;
    
    if(NULL == (fp = popen(cmd,"r")))
    {
        return -1;
    }
    
    if(NULL != res)
    {
        memset(res,0,res_len);
        while(NULL != (fgets(res,res_len,fp)))
        {
            pclose(fp);
            return 0;
        }
    }
    
    pclose(fp);
    return 0;
}

void is_running()
{
    int ret = -1;
    int fd = -1;
    int len = 0;
    char lock_file[256] = {0};
    char buf[32] = {0};
    struct flock lock;
    
    memset(lock_file,0,sizeof(lock_file));
    snprintf(lock_file,sizeof(lock_file),"%s/%s.%d","/var/run","nfqueue_backup_accept",0);
    
    do
    {
        fd = open(lock_file, O_WRONLY | O_CREAT, 0666);
        if(0 > fd)
        {
            break;
        }
        
        bzero(&lock,sizeof(lock));
        if(0 > fcntl(fd,F_GETLK,&lock))
        {
            break;
        }
        
        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_SET;
        
        if(0 > fcntl(fd,F_SETLK,&lock))
        {
            break;
        }
        
        memset(buf,0,sizeof(buf));
        len = snprintf(buf,sizeof(buf),"%d\n",(int)getpid());
        write(fd,buf,len);
        
        ret = 0;
    }while(0);
    
    if(0 != ret)
    {
        exit(1);
    }
}

static int cb(struct nfq_q_handle *qh, struct nfgenmsg *nfmsg,
        struct nfq_data *nfa, void *data)
{
    u_int32_t id = 0;
    struct nfqnl_msg_packet_hdr *ph;

    ph = nfq_get_msg_packet_hdr(nfa);
    if (ph) {
        id = ntohl(ph->packet_id);
    }

    return nfq_set_verdict(qh, id, NF_ACCEPT, 0, NULL);
}

void create_nfqueue_rcv()
{
    struct nfq_handle *h;
    struct nfq_q_handle *qh;
    struct nfnl_handle *nh;
    int fd;
    int rv;
    unsigned char buf[65535];
    
    h = nfq_open();
    nfq_unbind_pf(h, AF_INET);
    nfq_bind_pf(h, AF_INET);

    qh = nfq_create_queue(h, 0, &cb, NULL);
    nfq_set_mode(qh, NFQNL_COPY_PACKET, 0xffff);

    fd = nfq_fd(h);
    
    while ((rv = recv(fd, buf, sizeof(buf), 0)) && rv >= 0) {
        nfq_handle_packet(h, buf, rv);
    }

    nfq_destroy_queue(qh);
    nfq_close(h);
}

int main(int argc, char **argv)
{
    int pid;
    int ret;
    int state;
    char cmd_res[1024];

    while(1)
    {
        ret = exec_cmd("cat /proc/net/netfilter/nfnetlink_queue",cmd_res,sizeof(cmd_res));
        if(0 != ret || 0 == strlen(cmd_res))
        {
            pid = fork();
            switch(pid)
            {
                case 0 :
                    is_running();
                    create_nfqueue_rcv();
                case -1:
                default:
                    break;
            }
        }
        
        usleep(500);
    }
    return 0;
}