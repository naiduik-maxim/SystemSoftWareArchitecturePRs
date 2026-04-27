#include <stdlib.h>

int main() {
    
    system("echo 'test' > file.txt");
    system("sudo cp file.txt file_copy.txt");
    system("echo 'change' >> file_copy.txt");
    system("rm file_copy.txt");

    return 0;
}