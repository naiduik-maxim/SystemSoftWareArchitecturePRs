#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

int main() {
    int fd_c2s, fd_s2c, log_fd;
    char *fifo_c2s = "/tmp/fifo_c2s";
    char *fifo_s2c = "/tmp/fifo_s2c";
    char buf[1024];

    if (mkfifo(fifo_c2s, 0666) == -1 && errno != EEXIST) {
        perror("mkfifo c2s");
        exit(EXIT_FAILURE);
    }
    
    if (mkfifo(fifo_s2c, 0666) == -1 && errno != EEXIST) {
        perror("mkfifo s2c");
        exit(EXIT_FAILURE);
    }

    fd_c2s = open(fifo_c2s, O_RDONLY);
    if (fd_c2s < 0) {
        perror("open fifo_c2s");
        exit(EXIT_FAILURE);
    }

    log_fd = open("server.log", O_CREAT | O_WRONLY | O_APPEND, 0666);
    if (log_fd < 0) {
        perror("open server.log");
        exit(EXIT_FAILURE);
    }

    int bytes = read(fd_c2s, buf, sizeof(buf));
    if (bytes < 0) {
        perror("read fifo_c2s");
        exit(EXIT_FAILURE);
    } else if (bytes > 0) {
        if (write(log_fd, buf, bytes) < 0) {
            perror("write to log");
            exit(EXIT_FAILURE);
        }
        write(log_fd, "\n", 1);
    }

    fd_s2c = open(fifo_s2c, O_WRONLY);
    if (fd_s2c < 0) {
        perror("open fifo_s2c");
        exit(EXIT_FAILURE);
    }

    char *response = "HTTP 200 OK.";
    if (write(fd_s2c, response, strlen(response)) < 0) {
        perror("write fifo_s2c");
        exit(EXIT_FAILURE);
    }

    close(fd_c2s);
    close(fd_s2c);
    close(log_fd);

    unlink(fifo_c2s);
    unlink(fifo_s2c);

    return 0;
}