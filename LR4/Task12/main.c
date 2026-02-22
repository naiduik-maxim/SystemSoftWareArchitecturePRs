#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
    void *start = sbrk(0);
    printf("Initial brk: %p\n", start);

    for (int i = 0; i < 100; i ++){
        malloc(3);
    }

    void *after = sbrk(0);
    printf("After mallocs: %p\n", after);
    return 0;
}