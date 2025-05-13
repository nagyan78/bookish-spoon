#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 10

// 共享缓冲区
int buffer[BUFFER_SIZE];
int in = 0;  // 生产者插入数据的索引
int out = 0; // 消费者取出数据的索引

// 信号量
sem_t empty; // 表示缓冲区空闲的槽位数
sem_t full;  // 表示缓冲区已满的槽位数
pthread_mutex_t mutex; // 互斥锁，保护对缓冲区的访问

// 生产者线程函数
void* producer(void* arg) {
    while (1) {
        int item = rand() % 100; // 生产一个随机数据
        sem_wait(&empty);        // 等待空闲槽位
        pthread_mutex_lock(&mutex); // 加锁
        buffer[in] = item;       // 将数据放入缓冲区
        in = (in + 1) % BUFFER_SIZE; // 更新索引
        printf("生产者生产数据 %d 放入缓冲区位置 %d\n", item, in);
        pthread_mutex_unlock(&mutex); // 解锁
        sem_post(&full);       // 释放一个满槽位
        sleep(rand() % 2);     // 模拟生产时间
    }
    return NULL;
}

// 消费者线程函数
void* consumer(void* arg) {
    while (1) {
        sem_wait(&full);        // 等待有数据的槽位
        pthread_mutex_lock(&mutex); // 加锁
        int item = buffer[out]; // 从缓冲区取出数据
        out = (out + 1) % BUFFER_SIZE; // 更新索引
        printf("消费者消费数据 %d 从缓冲区位置 %d\n", item, out);
        pthread_mutex_unlock(&mutex); // 解锁
        sem_post(&empty);       // 释放一个空槽位
        sleep(rand() % 3);      // 模拟消费时间
    }
    return NULL;
}

int main() {
    // 初始化信号量和互斥锁
    sem_init(&empty, 0, BUFFER_SIZE); // 初始化空槽位信号量
    sem_init(&full, 0, 0);            // 初始化满槽位信号量
    pthread_mutex_init(&mutex, NULL); // 初始化互斥锁

    // 创建生产者和消费者线程
    pthread_t prod_thread, cons_thread;
    pthread_create(&prod_thread, NULL, producer, NULL);
    pthread_create(&cons_thread, NULL, consumer, NULL);

    // 等待线程结束（实际不会结束）
    pthread_join(prod_thread, NULL);
    pthread_join(cons_thread, NULL);

    // 销毁信号量和互斥锁
    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);

    return 0;
}
