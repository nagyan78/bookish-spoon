#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
    int pid;
    int pipe1[2];
    int pipe2[2];
    int x;

    // 创建两个管道
    if (pipe(pipe1) < 0) {
        perror("Failed to create pipe1");
        exit(EXIT_FAILURE);
    }
    if (pipe(pipe2) < 0) {
        perror("Failed to create pipe2");
        exit(EXIT_FAILURE);
    }

    // 创建子进程
    pid = fork();
    if (pid < 0) {
        perror("Failed to create new process");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // 子进程
        close(pipe1[1]);  // 关闭 pipe1 的写端
        close(pipe2[0]);  // 关闭 pipe2 的读端

        x = 1;
        do {
            read(pipe1[0], &x, sizeof(int));  // 从 pipe1 读取数据
            printf("child %d read: %d\n", getpid(), x);
            x++;
            write(pipe2[1], &x, sizeof(int));  // 向 pipe2 写入数据
        } while (x <= 9);

        close(pipe1[0]);
        close(pipe2[1]);
        exit(0);  // 子进程退出
    } else {
        // 父进程
        close(pipe1[0]);  // 关闭 pipe1 的读端
        close(pipe2[1]);  // 关闭 pipe2 的写端

        x = 1;
        do {
            write(pipe1[1], &x, sizeof(int));  // 向 pipe1 写入数据
            read(pipe2[0], &x, sizeof(int));  // 从 pipe2 读取数据
            printf("parent %d read: %d\n", getpid(), x);
            x++;
        } while (x <= 9);

        close(pipe1[1]);
        close(pipe2[0]);

        wait(NULL);  // 等待子进程结束
    }

    return EXIT_SUCCESS;
}
