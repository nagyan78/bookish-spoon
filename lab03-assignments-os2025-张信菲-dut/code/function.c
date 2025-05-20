#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h> 

int fx(int x) {
    if (x == 1)
        return 1;
    return x * fx(x - 1);
}

int fy(int y) {
    if (y == 1 || y == 2)
        return 1;
    return fy(y - 1) + fy(y - 2);
}

int main() {
    pid_t pid_fx, pid_fy;
    int x, y;
    int pipe_fx[2], pipe_fy[2];
    int result_fx, result_fy;

    printf("Input x and y:\n");
    if (scanf("%d %d", &x, &y) != 2) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }

    // 创建管道
    if (pipe(pipe_fx) < 0 || pipe(pipe_fy) < 0) {
        perror("Failed to create pipes");
        exit(EXIT_FAILURE);
    }

    // 创建第一个子进程计算 f(x)
    pid_fx = fork();
    if (pid_fx < 0) {
        perror("Failed to create new process for f(x)");
        exit(EXIT_FAILURE);
    } else if (pid_fx == 0) {
        // 子进程计算 f(x)
        close(pipe_fx[0]);  // 关闭读端
        result_fx = fx(x);
        write(pipe_fx[1], &result_fx, sizeof(result_fx));  // 写入结果
        close(pipe_fx[1]);  // 关闭写端
        exit(0);
    }

    // 创建第二个子进程计算 f(y)
    pid_fy = fork();
    if (pid_fy < 0) {
        perror("Failed to create new process for f(y)");
        exit(EXIT_FAILURE);
    } else if (pid_fy == 0) {
        // 子进程计算 f(y)
        close(pipe_fy[0]);  // 关闭读端
        result_fy = fy(y);
        write(pipe_fy[1], &result_fy, sizeof(result_fy));  // 写入结果
        close(pipe_fy[1]);  // 关闭写端
        exit(0);
    }

    // 父进程等待两个子进程结束
    waitpid(pid_fx, NULL, 0);
    waitpid(pid_fy, NULL, 0);

    // 父进程从管道读取结果
    close(pipe_fx[1]);  // 关闭写端
    close(pipe_fy[1]);  // 关闭写端
    read(pipe_fx[0], &result_fx, sizeof(result_fx));  // 读取 f(x) 的结果
    read(pipe_fy[0], &result_fy, sizeof(result_fy));  // 读取 f(y) 的结果
    close(pipe_fx[0]);  // 关闭读端
    close(pipe_fy[0]);  // 关闭读端

    // 父进程计算 f(x) + f(y)
    printf("f(x, y) = %d\n", result_fx + result_fy);

    return 0;
}
