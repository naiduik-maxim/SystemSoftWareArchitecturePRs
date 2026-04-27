#include <stdio.h>
#include <unistd.h>

int main(){
    int pid;
    pid = fork();
    printf("Pid: %d\n", pid);
    return 0;
}