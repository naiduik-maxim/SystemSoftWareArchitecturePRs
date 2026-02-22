#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>

int fib(int n){
    double arr[10000];

    if (n <= 1) return n;
    return fib(n - 1) + fib(n - 2);
}

int main(){
    struct rlimit rl;
    rl.rlim_cur = 8192; 
    rl.rlim_max = 16384;
    setrlimit(RLIMIT_STACK, &rl);

    printf("Result fib: %d", fib(40));
    return 0;
}