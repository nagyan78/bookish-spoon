#include <stdio.h> 
#include <sys/types.h> 
#include <unistd.h> 

int main() 
{
    pid_t child_pid; // 定义一个pid_t类型的变量，用于存储fork返回的子进程ID
    child_pid = fork(); // 调用fork()系统调用创建一个子进程
    // fork()返回值：
    // - 在父进程中返回子进程的PID（大于0）
    // - 在子进程中返回0
    // - 如果创建失败，返回-1

    if (child_pid < 0) // 如果fork调用失败
    {
        perror("Fork failed"); // 打印错误信息，perror会自动显示错误原因
        return 1; // 返回非零值表示程序异常退出
    }
    else if (child_pid == 0) // 如果当前是子进程（fork返回0）
    {
        printf("Child process: My PID is %d \n", getpid()); // 子进程打印自己的PID
    }
    else // 如果当前是父进程（fork返回子进程的PID）
    {
        printf("Parent process: My PID is %d \n", getpid()); // 父进程打印自己的PID
        printf("Parent process: Child process ID is %d \n", child_pid); // 父进程打印子进程的PID
    }
    return 0; 
}
