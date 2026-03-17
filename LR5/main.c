#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct A
{
    char a;
    int b;
};

struct B
{
    char a;
    int b;
} __attribute__((packed));


int main(){
    //struct A np = {'A', 128};
    //struct B tmp;

    //Valgrind
    struct A *np = malloc(sizeof(struct A));
    struct B *tmp = malloc(sizeof(struct B));
    
    np->a = 'A';
    np->b = 128;

    printf("Sizr of struct A(non packed): %zu\n", sizeof(struct A));
    printf("Sizr of struct B(packed): %zu\n", sizeof(struct B));

    memcpy(&tmp, &np, sizeof(struct A));

    free(np);
    free(tmp);
    return 0;
}