#include <stdio.h> 
#include <pthread.h> 
#include <unistd.h> 

// 线程函数
void* print_message(void* arg) {
    char* message = (char*)arg; // 将传入的参数转换为char*类型
    for (int i = 0; i < 5; i++) { // 循环5次
        printf("%s\n", message); // 打印传入的消息
        sleep(1); // 模拟耗时操作，线程暂停1秒
    }
    return NULL; // 线程函数返回NULL，表示线程正常结束
}

int main() {
    pthread_t thread1, thread2; // 定义两个pthread_t类型的变量，用于存储线程ID
    char* message1 = "Hello from Thread 1"; // 定义线程1的消息
    char* message2 = "Hello from Thread 2"; // 定义线程2的消息

    // 创建线程1
    if (pthread_create(&thread1, NULL, print_message, (void*)message1) != 0) {
        perror("Failed to create thread 1"); // 如果创建线程失败，打印错误信息
        return 1; // 返回非零值表示程序异常退出
    }
    // 创建线程2
    if (pthread_create(&thread2, NULL, print_message, (void*)message2) != 0) {
        perror("Failed to create thread 2"); // 如果创建线程失败，打印错误信息
        return 1; // 返回非零值表示程序异常退出
    }

    // 等待线程1结束
    pthread_join(thread1, NULL); // 主线程调用pthread_join等待线程1结束
    // 等待线程2结束
    pthread_join(thread2, NULL); // 主线程调用pthread_join等待线程2结束

    printf("Main thread finished\n"); // 打印主线程结束的消息
    return 0; 
}
