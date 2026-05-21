#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s \"Message\"\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int fd_c2s, fd_s2c;
    char *fifo_c2s = "/tmp/fifo_c2s";
    char *fifo_s2c = "/tmp/fifo_s2c";
    char buf[1024];

    fd_c2s = open(fifo_c2s, O_WRONLY);
    if (fd_c2s < 0) {
        perror("open fifo_c2s");
        exit(EXIT_FAILURE);
    }

    if (write(fd_c2s, argv[1], strlen(argv[1])) < 0) {
        perror("write fifo_c2s");
        exit(EXIT_FAILURE);
    }
    close(fd_c2s);

    fd_s2c = open(fifo_s2c, O_RDONLY);
    if (fd_s2c < 0) {
        perror("open fifo_s2c");
        exit(EXIT_FAILURE);
    }

    int bytes = read(fd_s2c, buf, sizeof(buf) - 1);
    if (bytes < 0) {
        perror("read fifo_s2c");
        exit(EXIT_FAILURE);
    } else if (bytes > 0) {
        buf[bytes] = '\0';
        printf("Server answer: %s\n", buf);
    }

    close(fd_s2c);

    return 0;
}