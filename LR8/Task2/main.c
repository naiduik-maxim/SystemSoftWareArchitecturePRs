#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(){
    int fd = open("test.bin", O_CREAT | O_RDWR | O_TRUNC, 0644);
    if(fd < 0){
        perror("open");
        return 1;
    }

    unsigned char data[] = {4, 5, 2, 2, 3, 3, 7, 9, 1, 5};
    write(fd, data, sizeof(data));

    lseek(fd, 3, SEEK_SET);

    unsigned char buffer[10];

    ssize_t n = read(fd, buffer, 4);
    if(n < 0){
        perror("read");
        return 1;
    }

    printf("Read: %zd bytes\n", n);

    for(int i = 0; i < n; i++){
        printf("%d ", buffer[i]);
    }
    printf("\n");

    close(fd);
    return 0;
}