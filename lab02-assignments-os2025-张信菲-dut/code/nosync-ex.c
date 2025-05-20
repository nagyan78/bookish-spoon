#include <pthread.h>
#include <stdio.h>

int sum = 0; // 全局变量，两个线程将对其累加

// 线程函数，对全局变量sum进行累加操作
void thread(void) {
    int i;
    for (i = 0; i < 1000000; i++) // 每个线程对sum累加1000000次
        sum += 1; // 累加操作，注意这里没有加锁，可能会导致数据竞争问题
}

int main(void) {
    pthread_t tid1, tid2; // 定义两个线程的线程ID

    // 创建两个线程，每个线程执行thread函数
    pthread_create(&tid1, NULL, thread, NULL);
    pthread_create(&tid2, NULL, thread, NULL);

    // 等待两个线程结束
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    // 打印最终的sum值，由于数据竞争，结果可能不正确
    printf("1000000 + 1000000 = %d\n", sum);

    return 0;
}
