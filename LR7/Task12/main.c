#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <syslog.h>
#include <time.h>

void log_program_start(const char *name){
    openlog(name, LOG_PID | LOG_CONS, LOG_USER);

    time_t now = time(NULL);
    char *time_str = ctime(&now);

    time_str[strcspn(time_str, "\n")] = 0;

    syslog(LOG_INFO, "Name: %s | PID: %d | Time %s", name, getpid(), time_str);
    printf("Name: %s | PID: %d | Time %s\n", name, getpid(), time_str);

    closelog();
}

int main(int argc, char *argv[]){
    log_program_start(argv[0]);

    sleep(2);
    return 0;
}
