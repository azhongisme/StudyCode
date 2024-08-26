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