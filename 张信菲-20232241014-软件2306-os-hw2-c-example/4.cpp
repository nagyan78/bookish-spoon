#include<stdio.h> 
#include<sys/types.h> 
#include<unistd.h> 

int main()
{
    fork(); // 第一次调用fork，创建一个子进程
    fork(); // 第二次调用fork，每个现有进程都会再创建一个子进程
    fork(); // 第三次调用fork，每个现有进程都会再创建一个子进程
    printf("Hello\n"); // 所有进程都会执行到这里，并打印"Hello"
    return 0; 
}
