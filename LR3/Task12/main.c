#include <stdio.h>
#include <sys/resource.h>
#include <string.h>

int main(){
    struct rlimit rl;
    rl.rlim_cur = 50;
    rl.rlim_max = 1024;
    setrlimit(RLIMIT_NOFILE, &rl);

    FILE *f[200];
    char name[20];
    char buf[10];
    for(int i = 0; i < 200; i++){
        strcpy(name, "file");

        itoa(i, buf, 10);
        strcat(name, buf);

        strcat(name, ".txt");

        if ((f[i] = fopen(name, "w")) == NULL){
            printf("Cannot open file %s", name);
        }

        if (f[i] == NULL) {
            printf("\n[!] Limit reached at %d\n", i);
            break;
        }   

        printf("Opened file %s\n", name);
    }

    return 0;
}