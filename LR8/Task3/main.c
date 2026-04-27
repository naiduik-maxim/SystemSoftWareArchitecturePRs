#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define N 1000000

int count = 0;
int cmp(const void *a, const void *b){
    int x = *(const int *)a;
    int y = *(const int *)b;
    count++;
    return (x > y) - (x < y); 
}

void gen_sorted(int *arr, int n){
    for(int i = 0; i < n; i++) arr[i] = i;
}

void gen_reversed(int *arr, int n){
    for(int i = 0; i < n; i++) arr[i] = n - i;
}

void gen_random(int *arr, int n){
    for(int i = 0; i < n; i++) arr[i] = rand();
}

void gen_same(int *arr, int n){
    for(int i = 0; i < n; i++) arr[i] = 67;
}

void gen_almost_sorted(int *arr, int n){
    gen_sorted(arr, n);
    for(int i = 0; i < n / 100; i++){
        int a = rand() % n;
        int b = rand() % n;

        int tmp = arr[a];
        arr[a] = arr[b];
        arr[b] = tmp;
    }
}

void test_sort(void (*gen)(int*, int), const char *name){
    int *arr = malloc(sizeof(int) * N);
    gen(arr, N);

    clock_t start = clock();
    qsort(arr, N, sizeof(int), cmp);
    clock_t end = clock();

    double time = (double)(end - start) / CLOCKS_PER_SEC;
    printf("%-20s: %.6f sec, count = %d\n", name, time, count);
    count = 0;
}

int main(){
    srand(time(NULL));

    test_sort(gen_random, "Random");
    test_sort(gen_sorted, "Sorted");
    test_sort(gen_reversed, "Reverse");
    test_sort(gen_same, "All equal");
    test_sort(gen_almost_sorted, "Almost sorted");

    return 0;
}