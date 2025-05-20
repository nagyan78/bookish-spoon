#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_CHAIRS 5 // 等待区的椅子数量

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // 互斥锁，保护共享变量
pthread_cond_t barber_sleep = PTHREAD_COND_INITIALIZER; // 条件变量，理发师等待顾客
pthread_cond_t customer_wait = PTHREAD_COND_INITIALIZER; // 条件变量，顾客等待理发师

int waiting_customers = 0; // 等待区的顾客数量

// 理发师线程函数
void* barber(void* arg) {
    while (1) { // 理发师不断工作
        pthread_mutex_lock(&mutex); // 锁定互斥锁

        // 如果没有顾客等待，理发师进入睡眠
        while (waiting_customers == 0) {
            printf("Barber is sleeping...\n");
            pthread_cond_wait(&barber_sleep, &mutex); // 等待顾客唤醒
        }

        // 有顾客等待，理发师开始理发
        printf("Barber is cutting hair...\n");
        waiting_customers--; // 减少等待区的顾客数量

        pthread_mutex_unlock(&mutex); // 解锁互斥锁

        sleep(rand() % 3 + 1); // 模拟理发时间
    }
}

// 顾客线程函数
void* customer(void* arg) {
    pthread_mutex_lock(&mutex); // 锁定互斥锁

    // 如果有空位，顾客坐下等待
    if (waiting_customers < NUM_CHAIRS) {
        waiting_customers++;
        printf("Customer takes a seat. Total waiting: %d\n", waiting_customers);
        pthread_cond_signal(&barber_sleep); // 唤醒理发师
    } else {
        // 如果没有空位，顾客离开
        printf("Customer leaves because no available chairs.\n");
    }

    pthread_mutex_unlock(&mutex); // 解锁互斥锁
    pthread_exit(NULL); // 顾客线程结束
}

int main() {
    pthread_t barber_thread; // 理发师线程
    pthread_t customer_threads[NUM_CHAIRS + 5]; // 顾客线程数组
    srand(time(NULL)); // 初始化随机数种子

    // 创建理发师线程
    pthread_create(&barber_thread, NULL, barber, NULL);

    // 创建顾客线程
    for (int i = 0; i < NUM_CHAIRS + 5; i++) {
        pthread_create(&customer_threads[i], NULL, customer, NULL);
    }

    // 等待理发师线程结束（理论上不会结束）
    pthread_join(barber_thread, NULL);

    // 等待所有顾客线程结束
    for (int i = 0; i < NUM_CHAIRS + 5; i++) {
        pthread_join(customer_threads[i], NULL);
    }

    return 0;
}
