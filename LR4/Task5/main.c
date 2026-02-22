#include <stdio.h>
#include <stdlib.h>

int main(){
    void *ptr = malloc(3);
    printf("ptr = %p\n", ptr);

    size_t size = -1;

    ptr = realloc(ptr, size);
    printf("ptr = %p\n", ptr);

    return 0;
}