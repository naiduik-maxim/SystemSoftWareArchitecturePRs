#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/resource.h>

int main(int argc, char *argv[]){
    struct rlimit rl;
    rl.rlim_cur = 16; 
    rl.rlim_max = 32;
    setrlimit(RLIMIT_FSIZE, &rl);

    if (argc != 3){
        printf("Program need two arguments!");
        return 1;
    }
    FILE *fin;
    FILE *fout;
    if ((fin = fopen(argv[1], "r")) == NULL){
        printf("Cannot open file %s for reading", argv[1]);
        return 1;
    }

    if ((fout = fopen(argv[2], "w")) == NULL){
        printf("Cannot open file %s for writing", argv[1]);
        return 1;
    }

    int tmp;
    while((tmp = getc(fin)) != EOF){
        if(putc(tmp, fout) == EOF){
            if (errno == EFBIG) printf("\n[!] Limit exceeded!\n");
            break;
        }
    }

    fclose(fout);
    fclose(fin);
    
    return 0;
}