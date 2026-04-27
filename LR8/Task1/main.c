#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#define SIZE 65536

int main(){
    int pipefd[2];
    char buffer[SIZE];

    memset(buffer, 'A', SIZE);

    if (pipe(pipefd) == -1){
        perror("pipe");
        return 1;
    }

    int flags = fcntl(pipefd[1], F_GETFD, 0);
    fcntl(pipefd[1], F_SETFL, flags | O_NONBLOCK);

    ssize_t total = 0;

    while(1){
        ssize_t count = write(pipefd[1], buffer, SIZE);

        if (count == -1){
            if(errno == EAGAIN || errno == EWOULDBLOCK){
                printf("Buffer pipe is full.");
                break;
            } else {
                perror("write");
                return 1;
            }
        }

        printf("Writen: %zd byte\n", count);
        total += count;

        if(count < SIZE){
            printf("Part write! count = %zd < %d\n", count, SIZE);
            break;
        }
    }

    printf("Total writen: %zd bytes\n", total);

    close(pipefd[0]);
    close(pipefd[1]);

    return 0;
}