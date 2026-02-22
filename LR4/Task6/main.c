#include <stdio.h>
#include <stdlib.h>

int main(){
    void *ptr = malloc(3);
    printf("ptr = %p\n", ptr);

    size_t size1 = 0;

    ptr = realloc(ptr, size1);
    printf("ptr = %p\n", ptr);

    ptr = malloc(3);
    ptr = realloc(ptr, (size_t)NULL);
    printf("sizeof(NULL): %ld\n", (size_t)NULL);
    printf("ptr = %p\n", ptr);

    return 0;
}