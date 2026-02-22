#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(){
    time_t t = (time_t)(__INT64_MAX__);

    printf("Sys time max: %ld\n", t);

    t++;

    printf("Sys time max + 1: %ld\n", t);


    return 0;
}