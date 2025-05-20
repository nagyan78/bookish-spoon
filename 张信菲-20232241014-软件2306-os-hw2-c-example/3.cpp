#include <stdio.h> // 包含标准输入输出头文件，用于printf和perror函数
#include <sys/types.h>
#include <unistd.h> 
#include <sys/wait.h> 

int main() 
{
    pid_t child_pid; // 定义一个pid_t类型的变量，用于存储fork返回的子进程ID
    child_pid = fork(); // 调用fork()系统调用创建一个子进程

    if (child_pid < 0) // 如果fork调用失败
    {
        perror("Fork failed"); // 打印错误信息，perror会自动显示错误原因
        return 1; // 返回非零值表示程序异常退出
    }
    else if (child_pid == 0) // 如果当前是子进程（fork返回0）
    {
        printf("Child process: My PID is %d \n", getpid()); // 子进程打印自己的PID
        // 子进程执行到这里会正常退出，返回0
    }
    else // 如果当前是父进程（fork返回子进程的PID）
    {
        printf("Parent process: Child process ID is %d \n", child_pid); // 父进程打印子进程的PID

        int status; // 定义一个变量，用于存储子进程的退出状态
        waitpid(child_pid, &status, 0); // 父进程调用waitpid等待子进程结束
        // 参数说明：
        // - child_pid：指定等待的子进程ID
        // - &status：存储子进程的退出状态
        // - 0：表示阻塞等待子进程结束

        if (WIFEXITED(status)) // 使用WIFEXITED宏检查子进程是否正常退出
        {
            printf("Parent process: Child exited with status %d\n", WEXITSTATUS(status)); 
            // 使用WEXITSTATUS宏获取子进程的退出状态码并打印
        }
    }
    return 0; 
}
