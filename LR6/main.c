#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "tpool.h"

void dummy_task(void *arg) {
    int id = *(int*)arg;
    usleep(50000); 
    free(arg);
}

int main() {
    printf("Starting ThreadSanitizer test...\n");

    tpool_t *tm = tpool_create(4);

    for (int i = 0; i < 10; i++) {
        int *arg = malloc(sizeof(int));
        *arg = i;
        tpool_add_work(tm, dummy_task, arg);
    }


    tpool_destroy(tm);

    printf("Test finished. If TSan is active, you should see warnings above.\n");
    return 0;
}