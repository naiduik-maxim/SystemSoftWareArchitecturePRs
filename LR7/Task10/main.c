#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(){
    srand(time(NULL));
    double num;

    printf("Input double number: ");
    scanf(" %lf", &num);

    printf("rand(0.0, 1.0): %f\n", (double)rand() / RAND_MAX);
    printf("rand(0.0, %f): %f\n", num, ((double)rand() / RAND_MAX) * num);

    return 0;
}