#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>

static struct nf_hook_ops nfho;

static unsigned int hook_func(
    const struct nf_hook_ops *ops, 
    struct sk_buff *skb,
    const struct net_device *in,
    const struct net_device *out,
    const struct nf_hook_state *state)
{
    char dmac[ETH_ALEN];

    struct ethhdr *eth_hdr = (struct ethhdr *)skb_mac_header(skb);
    if(skb_mac_header_was_set(skb))
    {
        memcpy(dmac, eth_hdr->h_dest, ETH_ALEN);
        printk("mac address : %s\n",dmac);
    }
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
