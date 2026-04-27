#include <stdio.h>
#include <unistd.h>
#include <sys/resource.h>
#include <sys/types.h>

int main() {
    struct rlimit limit;
    
    limit.rlim_cur = 1000;
    limit.rlim_max = 1500;

    if(setrlimit(RLIMIT_NPROC, &limit) != 0){
        perror("setrlimit failed");
        return 1;
    }

    int count = 0;

    while (1) {
        pid_t pid = fork();

        if(pid < 0){
            perror("fork failed");
            break;
        } else if (pid == 0) {
            sleep(100);
            return 0;
        } else {
            count++;
        }
    }

    printf("Process created: %d\n", count);
    return 0;
}