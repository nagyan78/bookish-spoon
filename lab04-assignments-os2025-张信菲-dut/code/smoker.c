/* Filename: smoker.c */
/* Function: 实现抽烟者问题 */

#include "smoker.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

// 获取IPC对象的ID
int get_ipc_id(char *proc_file, key_t key) {
    FILE *pf;
    int i, j;
    char line[BUFSZ], colum[BUFSZ];
    if ((pf = fopen(proc_file, "r")) == NULL) {
        perror("Proc file not open");
        exit(EXIT_FAILURE);
    }
    fgets(line, BUFSZ, pf);
    while (!feof(pf)) {
        i = j = 0;
        fgets(line, BUFSZ, pf);
        while (line[i] == ' ')
            i++;
        while (line[i] != ' ')
            colum[j++] = line[i++];
        colum[j] = '\0';
        if (atoi(colum) != key)
            continue;
        j = 0;
        while (line[i] == ' ')
            i++;
        while (line[i] != ' ')
            colum[j++] = line[i++];
        colum[j] = '\0';
        i = atoi(colum);
        fclose(pf);
        return i;
    }
    fclose(pf);
    return -1;
}

// 创建共享内存
char *set_shm(key_t shm_key, int shm_num, int shm_flag) {
    int shm_id;
    char *shm_buf;
    if ((shm_id = get_ipc_id("/proc/sysvipc/shm", shm_key)) < 0) {
        if ((shm_id = shmget(shm_key, shm_num, shm_flag)) < 0) {
            perror("shareMemory set error");
            exit(EXIT_FAILURE);
        }
        if ((shm_buf = (char *)shmat(shm_id, 0, 0)) < (char *)0) {
            perror("get shareMemory error");
            exit(EXIT_FAILURE);
        }
        for (int i = 0; i < shm_num; i++)
            shm_buf[i] = 0;
    } else {
        if ((shm_buf = (char *)shmat(shm_id, 0, 0)) < (char *)0) {
            perror("get shareMemory error");
            exit(EXIT_FAILURE);
        }
    }
    return shm_buf;
}

// 创建信号量
int set_sem(key_t sem_key, int sem_val, int sem_flag) {
    int sem_id;
    Sem_uns sem_arg;
    if ((sem_id = get_ipc_id("/proc/sysvipc/sem", sem_key)) < 0) {
        if ((sem_id = semget(sem_key, 1, sem_flag)) < 0) {
            perror("semaphore create error");
            exit(EXIT_FAILURE);
        }
        sem_arg.val = sem_val;
        if (semctl(sem_id, 0, SETVAL, sem_arg) < 0) {
            perror("semaphore set error");
            exit(EXIT_FAILURE);
        }
    }
    return sem_id;
}

// P操作（down）
int down(int sem_id) {
    struct sembuf buf;
    buf.sem_op = -1;
    buf.sem_num = 0;
    buf.sem_flg = SEM_UNDO;
    if (semop(sem_id, &buf, 1) < 0) {
        perror("down error");
        exit(EXIT_FAILURE);
    }
    return EXIT_SUCCESS;
}

// V操作（up）
int up(int sem_id) {
    struct sembuf buf;
    buf.sem_op = 1;
    buf.sem_num = 0;
    buf.sem_flg = SEM_UNDO;
    if (semop(sem_id, &buf, 1) < 0) {
        perror("up error");
        exit(EXIT_FAILURE);
    }
    return EXIT_SUCCESS;
}

// 抽烟者进程
void smoker(int smoker_id, int material1, int material2, int sem_id) {
    while (1) {
        down(sem_id); // 等待信号量
        if (material_ptr[material1] && material_ptr[material2]) {
            printf("Smoker %d is smoking using material %d and %d\n", smoker_id, material1, material2);
            material_ptr[material1] = 0; // 使用材料
            material_ptr[material2] = 0;
            sleep(1); // 模拟抽烟时间
        }
        up(sem_id); // 释放信号量
    }
}

// 供应者进程
void supplier(int supplier_id) {
    while (1) {
        int material1 = rand() % 3; // 随机选择两种材料
        int material2 = (material1 + 1) % 3;
        material_ptr[material1] = 1; // 提供材料
        material_ptr[material2] = 1;
        printf("Supplier %d provides material %d and %d\n", supplier_id, material1, material2);
        sleep(2); // 模拟供应时间
    }
}

int main() {
    // 初始化共享内存
    material_key = 101;
    material_num = 3; // 三种材料
    shm_flg = IPC_CREAT | 0644;
    material_ptr = (char *)set_shm(material_key, material_num, shm_flg);

    // 初始化信号量
    smoker1_sem_key = 201;
    smoker2_sem_key = 202;
    smoker3_sem_key = 203;
    sem_flg = IPC_CREAT | 0644;
    smoker1_sem = set_sem(smoker1_sem_key, 0, sem_flg);
    smoker2_sem = set_sem(smoker2_sem_key, 0, sem_flg);
    smoker3_sem = set_sem(smoker3_sem_key, 0, sem_flg);

    // 创建抽烟者进程
    if (fork() == 0) {
        smoker(1, 0, 1, smoker1_sem); // 抽烟者1需要材料0和1
        exit(0);
    }
    if (fork() == 0) {
        smoker(2, 1, 2, smoker2_sem); // 抽烟者2需要材料1和2
        exit(0);
    }
    if (fork() == 0) {
        smoker(3, 0, 2, smoker3_sem); // 抽烟者3需要材料0和2
        exit(0);
    }

    // 创建供应者进程
    if (fork() == 0) {
        supplier(1);
        exit(0);
    }
    if (fork() == 0) {
        supplier(2);
        exit(0);
    }

    // 父进程等待子进程结束
    for (int i = 0; i < 5; i++) {
        wait(NULL);
    }

    return 0;
}