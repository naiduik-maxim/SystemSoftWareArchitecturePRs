#include <stdio.h>
#include <stdlib.h>

/*int main(){
    void *ptr = NULL;
    int i = 0;
    while(i < 5){
        if(!ptr){
            ptr = malloc(3);
        }
        printf("ptr = %p\n", ptr);
        free(ptr);
        i++;
    }
    return 0;
}*/

int main(){
    void *ptr = NULL;
    int i = 0;
    while(i < 5){
        ptr = malloc(i);
        if(!ptr){
            return 1;
        }
        printf("ptr = %p\n", ptr);
        free(ptr);
        printf("ptr = %p\n", ptr);
        //ptr = NULL;
        i++;
    }
    return 0;
}