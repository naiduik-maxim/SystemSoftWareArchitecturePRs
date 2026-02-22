#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/resource.h>

void generate_values(int start, int end, int count, int *arr){
    for (int i = 0; i < count; i++){
        arr[i] = start +(rand() % end);
    }
}

int main(){
    struct rlimit rl;
    rl.rlim_cur = 1; 
    rl.rlim_max = 2;
    setrlimit(RLIMIT_CPU, &rl);

    srand(time(NULL));

    for (int i = 0; 1 ; i++){
        int arr1[6];
        int arr2[7];

        generate_values(1, 36, 6, arr1);
        generate_values(1, 49, 7, arr2);

        if (i % 100 == 0) {
            printf("\nArr1[6]: ");
            for (int j = 0; j < 6; j++){
                printf("%d ", arr1[j]);
            }
            printf("\nArr2[7]: ");
            for (int j = 0; j < 7; j++){
                printf("%d ", arr2[j]);
            }
            printf("Loop: %d", i);
        }

    }
    return 0;
}