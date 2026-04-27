#include <stdlib.h>

int main(){

    system("echo 'temp' > temp.txt");

    system("sudo chown root:root temp.txt");
    system("sudo chmod 600 temp.txt");

    system("cat temp.txt");
    system("echo 'new' >> temp.txt");

    return 0;
}