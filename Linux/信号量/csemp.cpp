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

