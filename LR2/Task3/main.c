#include <stdio.h>

int a;
int b = 2;

const int c = 100;

void func(){
    int arr1[1000];
    int arr2[1000] = {1};
}

int main(){
    int i;
    printf("The stack top is near %p\n", &i);
    printf("The a is near %p\n", &a);
    printf("The b is near %p\n", &b);
    printf("The c is near %p\n", &c);
    
    char arr[1000] = {'a'};
    printf("The char arr is near %p\n", &arr);

    printf("The func is near %p\n", func);
    return 0;
}