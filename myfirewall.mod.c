#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0xa683b406, "module_layout" },
	{ 0x5d3b55b5, "nf_unregister_hook" },
	{ 0xadc2290a, "nf_register_hook" },
	{ 0x50eedeb8, "printk" },
	{ 0xe2d5255a, "strcmp" },
	{ 0xb81960ca, "snprintf" },
	{ 0xb4390f9a, "mcount" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "C5FC0A5668FC078CFA8E69A");
