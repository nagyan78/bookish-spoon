/* Filename: smoker.h */
/* Function: 声明抽烟者问题中使用的IPC机制的函数原型和全局变量 */

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>

#define BUFSZ 256 // 缓冲区大小

// 声明IPC函数原型
int get_ipc_id(char *proc_file, key_t key);
char *set_shm(key_t shm_key, int shm_num, int shm_flag);
int set_sem(key_t sem_key, int sem_val, int sem_flag);
int down(int sem_id);
int up(int sem_id);

// 信号量联合体
typedef union semun {
    int val;
} Sem_uns;

// 共享变量
key_t material_key; // 材料共享内存键值
int material_num;   // 材料共享内存大小
char *material_ptr; // 材料共享内存指针

key_t smoker1_sem_key; // 抽烟者1的信号量键值
key_t smoker2_sem_key; // 抽烟者2的信号量键值
key_t smoker3_sem_key; // 抽烟者3的信号量键值
int smoker1_sem;       // 抽烟者1的信号量
int smoker2_sem;       // 抽烟者2的信号量
int smoker3_sem;       // 抽烟者3的信号量

int sem_flg;           // 信号量权限标志
int shm_flg;           // 共享内存权限标志