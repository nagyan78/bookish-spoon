#include<stdio.h> 
#include<sys/types.h> 
#include<unistd.h> 

int main()
{
    pid_t my_pid; // 定义一个pid_t类型的变量，用于存储进程ID
    my_pid = getpid(); // 调用getpid()系统调用，获取当前进程的ID，并将其赋值给my_pid
    printf("My process ID is %d\n", my_pid); // 使用printf函数输出当前进程的ID

    return 0; 
}
