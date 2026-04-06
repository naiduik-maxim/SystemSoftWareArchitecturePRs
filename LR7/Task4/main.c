#include <stdio.h>

int main(int argc, char *argv[]) {
    FILE *f = fopen(argv[1], "r");
    char line[10];
    int i = 0;

    while (fgets(line, sizeof(line), f)) {
        printf("%s", line);
        if (i == 10) {
            getchar();
            i = 0;
        }
        i++;
    }

    fclose(f);
    return 0;
}