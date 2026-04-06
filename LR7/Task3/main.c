#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]){
    if (argc < 3){
        printf("Usage: %s <word> <file>\n", argv[0]);
        return 1;
    }

    FILE *fin = fopen(argv[2], "r");
    if (!fin) {
        perror("fopen");
        return 1;
    }

    char buffer[255];

    while (fgets(buffer, sizeof(buffer), fin)) {
        if (strstr(buffer, argv[1])) {
            printf("%s", buffer);
        }
    }

    fclose(fin);
    return 0;
}