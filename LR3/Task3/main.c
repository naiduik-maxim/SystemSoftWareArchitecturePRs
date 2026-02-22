#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/resource.h>


int main(int argc, char* args[]){
    struct rlimit rl;
    rl.rlim_cur = 1024; 
    rl.rlim_max = 1024;
    setrlimit(RLIMIT_FSIZE, &rl);
    srand(time(NULL));

    FILE *file = fopen("res.txt", "w");
    if (file == NULL) {
        perror("Cannot open file");
        return 1;
    }

    for(int i = 0; i < 10000; i++){
        int res = (rand() % 6) + 1;
        if (fprintf(file, "%d", res) < 0){
            break;
        }

        if (fflush(file) != 0) {
            printf("\nLimit throw at throw %d!\n", i + 1);
            break;
        }
    }

    fclose(file);

    return 0;
}