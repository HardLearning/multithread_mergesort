#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#define DEBUG 0
#define array_length 1000000

int a[array_length+5];
int numofThread = 0;

void merge(int left, int right){
    int mid = (left + right) >> 1;
    int size1 = mid - left + 1;
    int size2 = right - mid;
    int t1[size1];
    int t2[size2];

    for (int i = 0; i < size1; i++) {
        t1[i] = a[left + i];
    }

    for (int j = 0; j < size2; j++) {
        t2[j] = a[mid + 1 + j];
    }

    int i = 0, j = 0;
    int k = left;
    while (i < size1 && j < size2) {
        if (t1[i] <= t2[j]) {
            a[k] = t1[i];
            i++;
        }
        else {
            a[k] = t2[j];
            j++;
        }
        k++;
    }

    while (i < size1) {
        a[k] = t1[i];
        k++;
        i++;
    }

    while (j < size2) {
        a[k] = t2[j];
        j++;
        k++;
    }
}


void merge_sort(void* arg){
    int *argu = (int*)arg;
    int left = argu[0];
    int right = argu[1];

    if (left < right) {
        int mid = (left + right) >> 1;


        int arg1[2];
        int arg2[2];

        
        arg1[0] = left;
        arg1[1] = mid;

        arg2[0] = mid + 1;
        arg2[1] = right;
        
        merge_sort(arg1);
        merge_sort(arg2);
        merge(left, right);
       
    }
}

void createData(){
    srand((int)time(NULL));
    for (int i = 0; i < array_length; ++i) {
        a[i] = rand();
        //printf("%d\n",a[i]);
    }
}

int main(){
    createData();

    struct timeval tbegin, tend;
    gettimeofday(&tbegin, NULL);

    int arg[2];
    arg[0] = 0;
    arg[1] = array_length;
    merge_sort(arg);
    gettimeofday(&tend, NULL);
    printf("The running time is %d milliseconds\n",(tend.tv_usec - tbegin.tv_usec) /1000);

    
#if DEBUG == 1
    for (int i = 0; i < array_length; ++i) {
        printf("%d\n",a[i]);
    }
#endif

    return 0;
}