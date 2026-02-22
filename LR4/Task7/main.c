#include <stdio.h>
#include <stdlib.h>

int main(){
    struct sbar {
        char a;
    }*ptr, *newptr;

    ptr = calloc(1000, sizeof(struct sbar));
    if (!ptr) return 1;
    printf("ptr = %p\n", ptr);

    //newptr = realloc(ptr, 5000 * sizeof(struct sbar));
    newptr = reallocarray(ptr, 5000, sizeof(struct sbar));
    
    if (!newptr) {
        free(ptr);
        return 1;
    }

    ptr = newptr;

    printf("newptr = %p\n", newptr);
    free(ptr);
    return 0;
}