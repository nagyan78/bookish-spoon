#include <pthread.h>
#include <stdio.h>

// 全局变量，两个线程将对它进行递增操作
int sum = 0;

// 定义一个互斥锁，用于保护对sum的访问
pthread_mutex_t mutex;

// 线程函数，对sum进行递增操作
void thread(void) {
    int i;
    for (i = 0; i < 1000000; i++) {
        pthread_mutex_lock(&mutex); // 加锁，确保对sum的访问是互斥的
        sum += 1; // 对sum进行递增操作
        pthread_mutex_unlock(&mutex); // 解锁，释放对sum的访问
    }
}

int main(void) {
    pthread_t tid1, tid2; // 定义两个线程ID

    // 初始化互斥锁
    pthread_mutex_init(&mutex, NULL);

    // 创建两个线程，每个线程都执行thread函数
    pthread_create(&tid1, NULL, thread, NULL);
    pthread_create(&tid2, NULL, thread, NULL);

    // 等待两个线程结束
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    // 打印最终的sum值
    printf("1000000 + 1000000 = %d\n", sum);

    // 销毁互斥锁（可选，程序即将结束时可以省略）
    pthread_mutex_destroy(&mutex);

    return 0;
}
