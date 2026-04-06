#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FILES 1024

int compare(const void *a, const void *b) {
    return strcmp(*(char **)a, *(char **)b);
}

int main(int argc, char *argv[]){
    struct dirent *pDirent;
    DIR *pDir;

    if (argc != 2){
        printf("Usage: %s <dirname>\n", argv[0]);
        return 1;
    }

    pDir = opendir(argv[1]);
    if(pDir == NULL){
        printf("Cannot open directory '%s'\n", argv[1]);
        return 1;
    }

    char *files[MAX_FILES];
    int count = 0;

    while((pDirent = readdir(pDir)) != NULL){
        files[count] = strdup(pDirent->d_name);
        count++;
    }

    closedir(pDir);

    qsort(files, count, sizeof(char *), compare);

    for(int i = 0; i < count; i++){
        printf("[%s]\n", files[i]);
        free(files[i]);
    }

    return 0;
}