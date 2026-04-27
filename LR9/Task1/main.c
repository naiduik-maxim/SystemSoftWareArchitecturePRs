#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
    FILE *fp = popen("getent passwd", "r");
    if (!fp) {
        perror("popen");
        return 1;
    }

    char buf[512];

    while(fgets(buf, sizeof(buf), fp)){
        char *token;
        int field = 0;
        int uid = 0;
        char username[255] = {0};

        token = strtok(buf, ":");
        while(token){
            if(field == 0) strcpy(username, token);
            if(field == 2) uid = atoi(token);

            token = strtok(NULL, ":");
            field++;
        }

        if(uid >= 500) printf("%s (UID=%d)\n", username, uid);
    }

    pclose(fp);
    return 0;
}