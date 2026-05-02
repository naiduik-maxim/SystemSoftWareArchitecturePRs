#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        return 1;
    }

    if (pid == 0) {
        execlp("echo", "echo", "Hello from child", NULL);

        perror("execlp failed");
        return 1;
    } else {
        wait(NULL);
        printf("Child process finished\n");
    }

    return 0;
}