#include <stdio.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include <stdlib.h> 

int main()
{
    int x = 1; // 定义一个整型变量x，并初始化为1
    pid_t p = fork(); // 调用fork()创建一个子进程
    // fork()返回值：
    // - 在父进程中返回子进程的PID（大于0）
    // - 在子进程中返回0
    // - 如果创建失败，返回-1

    if (p < 0) // 如果fork调用失败
    {
        perror("fork fail"); // 打印错误信息
        exit(1); // 退出程序，返回值为1表示异常退出
    }
    else if (p == 0) // 如果当前是子进程（fork返回0）
    {
        printf("Child has x = %d \n", ++x); // 子进程将x加1后打印
        // 注意：子进程中的x是父进程x的副本，修改子进程中的x不会影响父进程中的x
    }
    else // 如果当前是父进程（fork返回子进程的PID）
    {
        printf("Parent has x = %d\n", --x); // 父进程将x减1后打印
        // 注意：父进程中的x是独立的，修改父进程中的x不会影响子进程中的x
    }

    return 0; 
}
