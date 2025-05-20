#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>

int sum = 0; // 全局变量，两个线程将对其累加
sem_t sem;   // 信号量，用于同步线程对sum的访问

// 线程函数，对全局变量sum进行累加操作
void thread(void) {
    int i;
    for (i = 0; i < 1000000; i++) {
        sem_wait(&sem); // 等待信号量，进入临界区
        sum += 1;       // 对sum进行累加操作
        sem_post(&sem); // 释放信号量，离开临界区
    }
}

int main(void) {
    pthread_t tid1, tid2; // 定义两个线程的线程ID

    // 初始化信号量，初始值为1，表示一次只允许一个线程进入临界区
    sem_init(&sem, 0, 1);

    // 创建两个线程，每个线程执行thread函数
    pthread_create(&tid1, NULL, thread, NULL);
    pthread_create(&tid2, NULL, thread, NULL);

    // 等待两个线程结束
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    // 打印最终的sum值，由于使用了信号量，结果应该是正确的
    printf("1000000 + 1000000 = %d\n", sum);

    // 销毁信号量（可选，程序即将结束时可以省略）
    sem_destroy(&sem);

    return 0;
}
