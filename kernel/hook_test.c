#include "hook_test.h"

static struct nf_hook_ops nfho;

#if 1
static size_t _format_mac_addr(char *buf, int buflen,
    const unsigned char *addr, int len)
{
    int i;
    char *cp = buf;

    for (i = 0; i < len; i++) 
    {
        cp += scnprintf(cp, buflen - (cp - buf), "%02x", addr[i]);
        if (i == len - 1)
            break;
        cp += scnprintf(cp, buflen - (cp - buf), ":");
    }
    return cp - buf;
}
#endif 
static char *print_mac(char *buffer, const unsigned char *addr)
{
    _format_mac_addr(buffer, MAC_BUF_SIZE, addr, ETH_ALEN);          
    
    return buffer;
}

static unsigned int hook_func(
    const struct nf_hook_ops *ops, 
    struct sk_buff *skb,
    const struct net_device *in,
    const struct net_device *out,
    const struct nf_hook_state *state)
{
    u8 haddr[ETH_ALEN];
    char mac[MAC_BUF_SIZE];

    eth_header_parse(skb,haddr);
    print_mac(mac,haddr);
    printk("mac : %s\n",mac);
    return NF_ACCEPT;
}

static int __init hook_mod_init(void)
{
    nfho.hook = hook_func;
    nfho.hooknum = NF_INET_PRE_ROUTING;
    nfho.pf = PF_INET;
    nfho.priority = NF_IP_PRI_FIRST;

    nf_register_hook(&nfho);
    return 0;
}

static void __exit hook_mod_exit(void)
{
    nf_unregister_hook(&nfho);
}

module_init(hook_mod_init);
module_exit(hook_mod_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("mac filter");
MODULE_VERSION("alpha 1.0.0");
MODULE_AUTHOR("xuyi");
