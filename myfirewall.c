#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/skbuff.h>
#include <linux/tcp.h>
#include <linux/udp.h>
#include <linux/ip.h>
#include <linux/byteorder/generic.h>
#include <linux/inet.h>
#include <linux/in.h>
#include <linux/socket.h>

static struct nf_hook_ops ingress_hook; //netfilter hook for incoming packets
static struct nf_hook_ops egress_hook;	//netfilter hook for outgoing packets
struct sk_buff *sock_buff;		//bufer storing the packet
struct tcphdr *tcp_header;		//tcp header structure
struct iphdr *ip_header;		//ip header structure

unsigned int main_in_hook_funct(unsigned int hooknum,
 struct sk_buff **skb,
 const struct net_device *in,
 const struct net_device *out,
 int (*okfn)(struct sk_buff *)){
	sock_buff = skb;
	//grabing the headers
	ip_header = (struct iphdr *)skb_network_header(sock_buff);
	

	unsigned int short matchport = 23;	//telnet port
	char *blockedwebip="79.170.44.112";	//www.rahul-ravindran.com
	char *machinebip="10.0.2.5";
	char *machineaip="10.0.2.4";
	int size=15;
	char *sniffedsrc[15];	

	snprintf(sniffedsrc,size,"%pI4",&ip_header->saddr);
	
	if(!sock_buff){
		return NF_ACCEPT;	
	}
	if(ip_header->protocol==6){
		//Preventing machine B to telnet machine A
		if(strcmp(sniffedsrc,machinebip)==0){
			tcp_header = (struct tcphdr *)skb_transport_header(sock_buff);
			if((ntohs(tcp_header->dest)==matchport || ntohs(tcp_header->source)==matchport)==0){
				printk(KERN_INFO "Telnet connections not allowed.\n");	
				return NF_DROP;
			}
		}
	}
	if(ip_header->protocol==1){
		//Preventing any machine to ping to A
		printk(KERN_INFO "No ping Allowed to machine A\n");
		return NF_DROP;
	}
	return NF_ACCEPT;
}

unsigned int main_out_hook_funct(unsigned int hooknum,
 struct sk_buff **skb,
 const struct net_device *in,
 const struct net_device *out,
 int (*okfn)(struct sk_buff *)){
	sock_buff = skb;
	//grabing the ipheader
	ip_header = (struct iphdr *)skb_network_header(sock_buff);
	
	
	unsigned int short matchport = 23;	//telnet port
	char *blockedwebip="79.170.44.112";	//www.rahul-ravindran.com
	char *machinebip="10.0.2.5";
	char *machineaip="10.0.2.4";
	int size=15;
	char *sniffeddst[15];

	snprintf(sniffeddst,size,"%pI4",&ip_header->daddr);

	if(!sock_buff){
		return NF_ACCEPT;	
	}
	
	//Preventing telnet to machine B
	if(ip_header->protocol==6){
		if(strcmp(sniffeddst,machinebip)==0){
			tcp_header = (struct tcphdr *)skb_transport_header(sock_buff);
			if((ntohs(tcp_header->dest)==matchport || ntohs(tcp_header->source)==matchport)==0){
				printk(KERN_INFO "Telnet connection not allowed.\n");
				return NF_DROP;
			}
		}
	}

	//Prevent Machine a to ping to others
	if(ip_header->protocol==1){
		printk(KERN_INFO "No ping Allowed from this machine\n");
		return NF_DROP;
	}
	
	//Preventing machine A to connect to rahul-ravindran.com(79.170.44.112)
	if(strcmp(sniffeddst,blockedwebip)==0){
		printk(KERN_INFO "Accessed Blocked to www.rahul-ravindran.com\n");
		return NF_DROP;
	}
	return NF_ACCEPT;
}

int init_module(void){
//setting up the ingress filter
ingress_hook.hook = main_in_hook_funct;
ingress_hook.hooknum = NF_INET_LOCAL_IN;
ingress_hook.pf = PF_INET;
ingress_hook.priority = NF_IP_PRI_FIRST;
nf_register_hook(&ingress_hook);
//setting up the egress filter
egress_hook.hook = main_out_hook_funct;
egress_hook.hooknum = NF_INET_LOCAL_OUT;
egress_hook.pf = PF_INET;
egress_hook.priority = NF_IP_PRI_FIRST;
nf_register_hook(&egress_hook);

printk(KERN_INFO "hook was created\n");
return 0;
}

void cleanup_module(void){
nf_unregister_hook(&ingress_hook);
nf_unregister_hook(&egress_hook);
printk(KERN_INFO "hook was removed\n");
}
