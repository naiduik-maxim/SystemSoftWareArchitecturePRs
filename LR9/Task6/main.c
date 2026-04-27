#include <stdlib.h>

int main(){
    system("ls -l ~");
    system("ls -l /home/maxim/bin");
    system("ls -l /etc");

    system("cat /etc/shadow");
    system("touch /home/maxim/bin/testfile");
    
    return 0;
}