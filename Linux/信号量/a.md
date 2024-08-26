# 信号量
信号量本质上是一个非负数计数器，用于给共享资源建立一个标志，表示该共享资源被占用情况

# 操作
## p操作
将信号量的值减一，如果值为0，将阻塞等待直到值大于0
## v操作
将信号量的值加一，任何时候都不会阻塞

# 信号量的使用场景
如果约定信号量的取值只是0或1（0资源不可用，1资源可用），可以实现互斥锁
## csemp.h
```cpp
#ifndef CSEMP_H
#define CSEMP_H

#include <sys/types.h>
#include <sys/sem.h>

class csemp {
private:
    union semun //用于信号量操作的共同体
    {
        int val;
        struct semid_ds *buf;
        unsigned short *arry;
    };
    int semid; //信号量id

    //如果把sem_flg_设置为SEM_UNDO,操作系统将跟踪进程对信号量的修改情况
    //在全部修改信号量的进程终止后，操作系统将把信号量恢复成初始值
    //如果信号量用于互斥锁设置为SEM_UNDO
    //如果信号量用于生产消费者模型，设置为0
    short sem_flg_;

    csemp(const csemp&) = delete;
    csemp& operator = (const csemp&) = delete;
public:
    csemp() : semid(-1) {}
    //如果信号量已存在获取信号量，如果不存在，则创建它并初始化为value
    //如果用于互斥锁，value填1，sem_flg填SEM_UNDO
    //如果用于生产消费者模型，value填0，sem_flg填0
    bool init(key_t key, unsigned short value = 1, short sem_flg = SEM_UNDO);
    bool wait(short value = -1);
    bool post(short value = 1);
    int getValue();
    bool destory();
    ~csemp() {}
};
#endif
```
## csemp.cpp
```cpp
#include "csemp.h"
#include <errno.h>
#include <error.h>
#include <cstdio>

bool csemp::init(key_t key, unsigned short value, short sem_flg)
{
    if (semid != -1) return false;

    sem_flg_ = sem_flg;

    //信号量的初始化不能直接用semget(key, 1. 0666|IPC_CREAT)
    //因为信号量创建后，初始值是0，如果用于互斥锁，需要把初始值设置为1
    //而获取信号量不需要初始值，所以，创建信号量和获取信号量的流程不同

    //信号量初始化
    //1.获取信号量
    //2.如果失败，创建
    //3.设置信号量的初始值

    semid = semget(key, 1, 0666);
    if ( semid == -1)
    {
        if (errno == ENOENT) 
        {
            semid = semget(key, 1, 06666 | IPC_CREAT | IPC_EXCL);
            if (semid == -1) 
            {
                if (errno == EEXIST) 
                {
                    semid = semget(key, 1 , 0666);
                    if (semid == -1) 
                    {
                        perror("init 1 semget()"); return false;
                    }
                    return true;
                }
                else
                {
                    perror("init 2 semget()");return false;
                }
            }
            union semun sem_union;
            sem_union.val = value;
            if(semctl(semid, 0, SETVAL, sem_union) < 0)
            {
                perror("init semctl()");return false;
            }
        }
        else
        { perror("init 3 semget()"); return false;}
    }
    return true;
}

bool csemp::wait(short value) 
{
    if (semid == -1) return false;

    struct sembuf sem_b;
    sem_b.sem_num = 0;
    sem_b.sem_op = value;
    sem_b.sem_flg = sem_flg_;
    if (semop(semid, &sem_b, 1) == -1) {perror("p semop()"); return false;}

    return true;
}

bool csemp::post(short value)
{
    if (semid == -1) return false;

    struct sembuf sem_b;
    sem_b.sem_num = 0;
    sem_b.sem_op = value;
    sem_b.sem_flg = sem_flg_;
    if (semop(semid, &sem_b, 1) == -1) {perror("V semop()"); return false;}

    return true;
}

int csemp::getValue()
{
    return semctl(semid, 0, GETVAL);
}

bool csemp::destory()
{
    if (semid == -1) return false;

    if (semctl(semid, 0, IPC_RMID) == -1) {perror("destory semtcl()"); return false; }

    return true;
}
```
## main.cpp
```cpp
#include "csemp.h"
#include "csemp.cpp"
#include <iostream>
#include <cstring>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
struct stgirl{
    int no;
    char name[51];
};

int main(int argc, char* argv[]) {
    if (argc != 3) {
        return -1;
    }

    int shmid = shmget(0x5005, sizeof(stgirl), 0640|IPC_CREAT);

    if (shmid == -1) {
        std::cout << "shmget(0x5005) failed\n";
        return -1;
    }

    std::cout << "shmid = " << shmid << std::endl;

    stgirl *ptr = (stgirl *) shmat(shmid, 0, 0);
    if (ptr == (void *)-1) {
        std::cout << "shmat() filed\n";
        return -1;
    }
    
    csemp mutex;
    if (mutex.init(0x5005) == false) {
        std::cout << "mutex init failed\n";
        return -1;
    }

    std::cout << "apply for lgging\n";
    mutex.wait();

    std::cout << "原值： no = " << ptr->no << ", name = " << ptr->name << std::endl;
    ptr->no = atoi(argv[1]);
    strcpy(ptr->name, argv[2]);
    std::cout << "新值： no = " << ptr->no << ", name = " << ptr->name << std::endl;
    sleep(10);

    mutex.post();
    std::cout << "deblocking\n";

    shmdt(ptr);

    //删除共享内存
    if (shmctl(shmid, IPC_RMID, 0) == -1) {
        std::cout << "shmctl failed\n";
    }
}
```

如果约定信号量的取值表示可用资源的数量，可以实现生产、消费者模型
## 生产者
```cpp
#include "squeue.h"
#include "csemp.cpp"
#include <sys/ipc.h>
#include <sys/shm.h>

int main() {
    struct stgirl {
        int no;
        char name[51];
    };

    using ElemType = stgirl;

    int shmid = shmget(0x5005, sizeof(squeue<ElemType, 5>), 0640|IPC_CREAT);
    if (shmid == -1) {
        std::cout << "shmget failed\n"; return -1;
    }

    squeue<ElemType, 5> *sq = static_cast<squeue<ElemType, 5>*>(shmat(shmid, 0, 0));
    if (sq == (void*)-1) {
        std::cout << "shmat failed\n"; return -1;
    }

    sq->init();

    ElemType e;

    csemp mutex; mutex.init(0x5001);
    csemp cond; cond.init(0x5002, 0, 0);

    mutex.wait();

    e.no = 3; strcpy(e.name, "aa"); sq->push(e);
    e.no = 4; strcpy(e.name, "bb"); sq->push(e);
    e.no = 5; strcpy(e.name, "cc"); sq->push(e);

    mutex.post();
    cond.post(3);

    shmdt(sq);

}
```

## 消费者
```cpp
#include "squeue.h"
#include "csemp.cpp"
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
int main() {
    struct stgirl {
        int no;
        char name[51];
    };

    using ElemType = stgirl;

    int shmid = shmget(0x5005, sizeof(squeue<ElemType, 5>), 0640|IPC_CREAT);
    if (shmid == -1) {
        std::cout << "shmget failed\n";
        return -1;
    }
    
    squeue<ElemType, 5>* sq = static_cast<squeue<ElemType, 5>*>(shmat(shmid, 0, 0));
    if (sq == (void *)-1) {
        std::cout << "shmat() failed\n";
        return -1;
    }

    sq->init();

    ElemType e;

    csemp mutex; mutex.init(0x5001); //用于给共享内存加锁
    csemp cond; cond.init(0x5002, 0, 0); //信号量的值用于表示队列中数据元素的个数

    while (true) {
        mutex.wait();

        while (sq->empty()) {
            mutex.post(); //解锁
            cond.wait(); //等待生产者的唤醒信号
            mutex.wait(); //加锁
        }

        e =sq->front();
        sq->pop();
        mutex.post();

        std::cout << "no = " << e.no << ", name = " << e.name << std::endl;
        usleep(100);
    }

    shmdt(sq);
}
```

# 查看系统中存在的信号量
```shell
$ ipcs -s
```
# 删除
```shell
$ ipcrm sem 信号量id
```