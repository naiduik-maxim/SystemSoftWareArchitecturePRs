#include <stdio.h>
#include <stdlib.h>

int main(){
    int size = -1;
    void *p = malloc(size); //Перетворення на 2^64
    printf("p = %p\n", p);

    int a = 1024 ^ 1024;
    int b = 1024;

    int num = a * b;
    printf("num = %d\n", num);

    void *ptr = malloc(num);
    printf("ptr = %p\n", ptr);
    return 0;
}