#include <stdio.h>
#include <time.h>

int main(){
    clock_t start, end;
    double time;

    start = clock();

    for(int i = 0; i < 99999999; i++);

    end = clock();

    time = ((double)(end-start)) * 1000 / CLOCKS_PER_SEC;

    printf("Time: %f\n", time);

    return 0;
}