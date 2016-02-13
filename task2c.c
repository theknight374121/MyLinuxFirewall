#define __KERNEL__
#define MODULE

#include <linux/module.h>
#include <linux/kernel.h>

int init_module(void){
printk(KERN_INFO "init_module() was called\n");
return 0;
}

void cleanup_module(void){
printk(KERN_INFO "cleanup_module() was called\n");
}
