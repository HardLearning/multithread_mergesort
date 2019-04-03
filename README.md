# C语言实现多线程的归并排序

## 问题

​	利用多线程实现归并排序。归并排序是分治算法的代表，适合改写为多线程。



## 构造数据

​	数据规模为$2*10^6$,整数。$array\_length = 2*10^6$

```c
    srand((int)time(NULL));
    for (int i = 0; i < array_length; ++i) {
        a[i] = rand();
    }
```



## 计时

​	利用$<sys/time.h>$内部提供的$getnumofday()$函数，可以精确到微妙级别，但我最后输出是毫秒级别，所以需要转换单位。代码如下：

```c
struct timeval tbegin, tend;
    gettimeofday(&tbegin, NULL);

    int arg[2];
    arg[0] = 0;
    arg[1] = array_length;

    pthread_t  tid;
    numofThread = 1;
    pthread_create(&tid, NULL, merge_sort, arg);
    pthread_join(tid, NULL);
    gettimeofday(&tend, NULL);
    if (flag == 1) {
       printf("The number of thread that I use : %d\n", maxThreadNumber);     
    }
```



## 归并排序

​	我需要控制好线程的使用个数，因此要统计当前正在运行的线程个数。为了避免潜在的bug，我并没有用到多少指针的东西，基本都是全局数组全局变量。归并排序其实有两个函数，$merge\_sort$函数是递归实现分治的基础。我在该函数的单线程版本上进行了些许改动。

​	代码如下：

```c
void merge_sort(void* arg){
    int *argu = (int*)arg;
    int left = argu[0];
    int right = argu[1];

    int mid = (left + right) >> 1;
    int arg1[2];
    int arg2[2];

        
    arg1[0] = left;
    arg1[1] = mid;

    arg2[0] = mid + 1;
    arg2[1] = right;

    if (left >= right) {
        return;
    }

    pthread_t t2;
    pthread_t t1;

    if (numofThread == maxThreadNumber) {
        flag = 1;
    }
    
    if (numofThread < maxThreadNumber) {
        numofThread += 1; 
        pthread_create(&t1, NULL, merge_sort, arg1);
        pthread_join(t1, NULL); 
        pthread_exit(NULL);
        numofThread -= 1;
    }
    else {
        merge_sort(arg1);
    }
    
    if (numofThread < maxThreadNumber) {
        numofThread += 1;
        pthread_create(&t2, NULL, merge_sort, arg2); 
        pthread_join(t2, NULL);
        pthread_exit(NULL);
        numofThread -= 1;
    }
    else {
        merge_sort(arg2);
    }
    
    merge(left, right);
    
}

```



## 完整代码

```c
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#define DEBUG 0
#define array_length 2000000

int a[array_length+5];
int numofThread = 0;
int maxThreadNumber = 21;
int flag = 0;


void merge(int left, int right){
    int mid = (left + right) >> 1;
    int size1 = mid - left + 1;
    int size2 = right - mid;
    int t1[size1];
    int t2[size2];

    memcpy(t1, a+left, sizeof(int) * (mid-left+1));
    memcpy(t2, a+mid+1, sizeof(int) * (right-mid));

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

    int mid = (left + right) >> 1;
    int arg1[2];
    int arg2[2];

        
    arg1[0] = left;
    arg1[1] = mid;

    arg2[0] = mid + 1;
    arg2[1] = right;

    if (left >= right) {
        return;
    }

    pthread_t t2;
    pthread_t t1;

    if (numofThread == maxThreadNumber) {
        flag = 1;
    }
    
    if (numofThread < maxThreadNumber) {
        numofThread += 1; 
        pthread_create(&t1, NULL, merge_sort, arg1);
        pthread_join(t1, NULL); 
        pthread_exit(NULL);
        numofThread -= 1;
    }
    else {
        merge_sort(arg1);
    }
    
    if (numofThread < maxThreadNumber) {
        numofThread += 1;
        pthread_create(&t2, NULL, merge_sort, arg2); 
        pthread_join(t2, NULL);
        pthread_exit(NULL);
        numofThread -= 1;
    }
    else {
        merge_sort(arg2);
    }
    
    merge(left, right);
    
}

void createData(){
    srand((int)time(NULL));
    for (int i = 0; i < array_length; ++i) {
        a[i] = rand();
    }
}

int main(){
    createData();
    
    struct timeval tbegin, tend;
    gettimeofday(&tbegin, NULL);

    int arg[2];
    arg[0] = 0;
    arg[1] = array_length;

    pthread_t  tid;
    numofThread = 1;
    pthread_create(&tid, NULL, merge_sort, arg);
    pthread_join(tid, NULL);
    gettimeofday(&tend, NULL);
    if (flag == 1) {
       printf("The number of thread that I use : %d\n", maxThreadNumber);     
    }
    
    printf("The running time is %d millisecond\n",(tend.tv_usec - tbegin.tv_usec)/1000);

    
#if DEBUG == 1
    for (int i = 0; i < array_length; ++i) {
        printf("%d\n",a[i]);
    }
#endif

    return 0;
}
```



## 性能分析

​	下面比较各种线程数量，我的程序的运行效果。

![single_thread](/media/wyh-dr/_dde_data/OS_bonus/single_thread.png)

![thread_2](/media/wyh-dr/_dde_data/OS_bonus/thread_2.png)

![thread_4](/media/wyh-dr/_dde_data/OS_bonus/thread_4.png)

![thread_8](/media/wyh-dr/_dde_data/OS_bonus/thread_8.png)

​	从这4个不同的线程数量可以对比看出，线程数越多，程序运行时间是越短的。单线程运行效率最低，多线程每次线程数翻倍，运行时间大大缩短。而且并不是只缩短一半，缩短幅度大大提高。

​	

## 总结

​	归并排序并不难，改写为多线程版本花费了不少时间在调试代码上。说明我对多线程的掌握还不算到位。但是通过这次练习，我有了更深刻的了解。





​	