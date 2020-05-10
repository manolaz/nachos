#include <stdio.h>
#include <unistd.h>
#include <syslog.h>

int main(void) {

 openlog("slog", LOG_PID|LOG_CONS, LOG_USER);
 syslog(LOG_EMERG, "Hello from my code ");
 closelog();

 return 0;
}
