#include <linux/kernel.h>
#include <linux/module.h>
#include <syslog.h>
#include <main_int.h>



int hello_init(void)
{
syslog(LOG_EMERG,"Hello world\n");
syslog(LOG_EMERG,"un autre message au chargement\n");
return 0;
}

void hello_exit(void)
{
syslog(LOG_EMERG,"Bye world\n");
syslog(LOG_EMERG,"un autre message au déchargement\n");
}

module_init(hello_init);
module_exit(hello_exit);
//MODULE_LICENSE("GPL");

