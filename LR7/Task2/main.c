#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

void my_print(__mode_t mode) {
    printf((S_ISDIR(mode)) ? "d" : "-");
    printf((mode & S_IRUSR) ? "r" : "-");
    printf((mode & S_IWUSR) ? "w" : "-");
    printf((mode & S_IXUSR) ? "x" : "-");
    printf((mode & S_IRGRP) ? "r" : "-");
    printf((mode & S_IWGRP) ? "w" : "-");
    printf((mode & S_IXGRP) ? "x" : "-");
    printf((mode & S_IROTH) ? "r" : "-");
    printf((mode & S_IWOTH) ? "w" : "-");
    printf((mode & S_IXOTH) ? "x" : "-");
}

int main() {
    DIR *dir = opendir(".");
    struct dirent *entry;
    struct stat file_stat;

    while ((entry = readdir(dir)) != NULL) {
        stat(entry->d_name, &file_stat);
        my_print(file_stat.st_mode);
        printf(" %ld", file_stat.st_nlink);
        printf(" %s", getpwuid(file_stat.st_uid)->pw_name);
        printf(" %s", getgrgid(file_stat.st_gid)->gr_name);
        printf(" %5ld", file_stat.st_size);
        
        char date[20];
        strftime(date, 20, "%b %d %H:%M", localtime(&file_stat.st_mtime));
        printf(" %s %s\n", date, entry->d_name);
    }
    closedir(dir);
    return 0;
}