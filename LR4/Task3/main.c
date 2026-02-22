#include <stdio.h>
#include <stdlib.h>

int main(){
    void *p = malloc(0);
    printf("p = %p\n", p);
    return 0;
}