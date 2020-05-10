#include <linux/kernel.h>
#include <linux/module.h>
//#include <linux/printk.h>
int
hello_init(void)
{
	printk("Hello world\n");
	printk("un autre message au chargement\n");
	return 0;
}

void
hello_exit(void)
{
	printk("Bye world\n");
	printk("un autre message au déchargement\n");
}

module_init(hello_init);
module_exit(hello_exit);
//MODULE_LICENSE("GPL");

