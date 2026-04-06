#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>

int main() {
    DIR *pDir = opendir(".");
    struct dirent *entry;
    char choice;

    if (!pDir) {
        perror("opendir");
        return 1;
    }

    while((entry = readdir(pDir)) != NULL) {

        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        char *dot = strrchr(entry->d_name, '.');

        if(dot && strcmp(dot, ".c") == 0){
            printf("Delete %s (Y/n): ", entry->d_name);

            scanf(" %c", &choice);

            if(choice == 'Y'){
                remove(entry->d_name);
            }
        }
    }

    closedir(pDir);
    return 0;
}