#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

#define PATH "test.txt"

int main(){
    pid_t pid = fork();

    if (pid < 0){
        perror("fork");
        return 1;
    }

    if(pid == 0){
        int wfd = open(PATH, O_WRONLY, 0644);
        if(wfd < 0){
            perror("open");
            return 1;
        }

        const char *data[] = {
            "AAAAAA\n",
            "BBBBBB\n",
            "CCCCCC\n",
            "DDDDDD\n",
            "FFFFFF\n"
        };

        for(int i = 0; i < 5; i++){
            write(wfd, data[i], strlen(data[i]));
            printf("WRITER write: %s", data[i]);
            sleep(1);
        }

        close(wfd);
        exit(0);

    } else {
        int rfd = open(PATH, O_RDONLY, 0644);
        if(rfd < 0){
            perror("open");
            return 1;
        }

        char buffer[32];

        for(int i = 0; i < 5; i++){
            sleep(1);

            lseek(rfd, 0, SEEK_SET);
            int n = read(rfd, buffer, sizeof(buffer) - 1);

            if(n > 0){
                buffer[n] = '\0';
                printf("READER read (%d bytes):\n %s\n", n, buffer);
            } else {
                printf("READER nothing read\n");
            }
        }

        wait(NULL);
        close(rfd);
    }

    return 0;
}