#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef enum {cchar, iint, ddouble} type;

int swap(void* a, void* b, size_t size){
    void* tmp = malloc(size);
    
    if (tmp == NULL) return 0;

    memcpy(tmp, a, size);
    memcpy(a, b, size);
    memcpy(b, tmp, size);

    free(tmp);
    return 1;
}

int compare(void* a, void* b, type type) {
    if (type == iint) {
        if (*(int*)a < *(int*)b) return -1;
        if (*(int*)a > *(int*)b) return 1;
    } else if (type == ddouble) {
        if (*(double*)a < *(double*)b) return -1;
        if (*(double*)a > *(double*)b) return 1;
    } else if (type == cchar) {
        if (*(char*)a < *(char*)b) return -1;
        if (*(char*)a > *(char*)b) return 1;
    }
    return 0;
}

int partition(void *arr, int low, int high, size_t size, type type){
    char* pi = (char*)arr; 
    void* pivot = pi + high * size; 
    int i = low - 1;
    int j;

    for (j = low; j < high; j++) {
        if (compare(pi + j * size, pivot, type) < 0) {
            i++;
            swap(pi + i * size, pi + j * size, size);
        }
    }
    swap(pi + (i + 1) * size, pi + high * size, size);
    return (i + 1);
}

void quick_sort(void *arr, int low, int high, size_t size, type type){
    if (low < high){
        int pi = partition(arr, low, high, size, type);

        quick_sort(arr, low, pi - 1, size, type);
        quick_sort(arr, pi + 1, high, size, type);
    }
}

int fill_array(void *arr, int size, type type){
    for(int i = 0; i < size; i++){
        if(type == cchar){
            *((char*)arr + i) = 'a' + rand() % 26;
        } else if (type == iint){
            *((int*)arr + i) = rand() % 1000;
        } else {
            *((double*)arr + i) = (double)((rand() % 1000) / 10.0);
        }
    }
    return 1;
}

int find_element(void *arr, int size, int index, void *element, size_t size_el ,type type){
    if(index >= size) return -1;
    char* pi = (char *)arr;
    
    if(compare(pi + size_el * index, element, type) == 0) return index;
    return find_element(arr, size, index + 1, element, size_el, type);
}

int binary_search(void *arr, int low, int high, void* target, size_t size_el, type type) {
    if (low > high) return -1;
    int mid = low + (high - low) / 2;
    char* pi = (char*)arr;
    int res = compare(pi + mid * size_el, target, type);

    if (res == 0) return mid;
    if (res > 0) return binary_search(arr, low, mid - 1, target, size_el, type);
    return binary_search(arr, mid + 1, high, target, size_el, type);
}

void print_array(void* arr, int size, type type, size_t size_el){
    char* pi = (char*)arr;
    for (int i = 0; i < size; i++){
        if(type == iint) printf("%d ", *(int*)(pi + i*size_el));
        else if(type == ddouble) printf("%.2f ", *(double*)(pi + i*size_el));
        else printf("%c ", *(char*)(pi + i*size_el));
    }
}

int main(int argc, char *argv[]){
    if (argc != 2){
        printf("Input correct type of array (int|char|double)");
        return 1;
    }

    srand(time(NULL));
    int size_array = 10;
    type type;
    size_t size_el;
    void* serch = malloc(8);

    if(strcmp(argv[1], "int") == 0) { 
        type = iint; size_el = sizeof(int);
    } else if(strcmp(argv[1], "double") == 0) { 
        type = ddouble; size_el = sizeof(double); 
    } else { 
        type = cchar; size_el = sizeof(char);
    }

    void* arr = malloc(size_array * size_el);
    fill_array(arr, size_array, type);

    printf("\nArray: ");
    print_array(arr, size_array, type, size_el);

    printf("\nFind element: ");

    if (type == iint) {
        scanf("%d", (int*)serch);
    } else if (type == ddouble) {
        scanf("%lf", (double*)serch);
    } else if (type == cchar) {
        scanf(" %c", (char*)serch);
    }

    int index = 0;
    index = find_element(arr, size_array, index, serch, size_el, type);
    if (index != -1){
        printf("\nElement index %d recursion serch\n", index);
    }
    else printf("\nElement nit found\n");

    quick_sort(arr, 0, size_array - 1, size_el, type);
    printf("\nSorted array:");
    print_array(arr, size_array, type, size_el);

    index = binary_search(arr, 0, size_array - 1, serch, size_el, type);

    if (index != -1){
        printf("\nElement index %d binary serch\n", index);
    }
    else printf("\nElement nit found\n"); 

    free(arr);
    free(serch);

    return 0;
}