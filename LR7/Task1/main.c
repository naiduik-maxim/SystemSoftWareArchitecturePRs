#include <stdio.h>

int main(){
    FILE *fin, *fout;
    char buffer[255];

    fin = popen("who", "r");
    fout = popen("more", "w");

    if (fin && fout){
        while (fgets(buffer, sizeof(buffer), fin)){
            fputs(buffer, fout);
        }

        pclose(fin);
        pclose(fout);
    }

    return 0;
}
