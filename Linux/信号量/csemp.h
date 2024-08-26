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