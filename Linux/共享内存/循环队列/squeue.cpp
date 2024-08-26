#include "squeue.h"
#include <sys/ipc.h>
#include <sys/shm.h>


int main() {
    using ElemType = int;

    int shmid = shmget(0x5005, sizeof(squeue<ElemType, 5>), 0640|IPC_CREAT);
    if (shmid == -1) {
        std::cout << "shmget failed\n";
        return -1;
    }

    squeue<ElemType, 5>* sq = (squeue<ElemType, 5> *)shmat(shmid, 0, 0);
    if (sq == (void *)-1) {
        std::cout << "shmat failed\n";
        return -1;
    }

    sq->init();

    ElemType e;
    e = 1;
    sq->push(e);
    e = 2;
    sq->push(e);
    e = 3;
    sq->push(e);
    sq->printQueue();

    shmdt(sq);

    if (shmctl(shmid, IPC_RMID, 0) == -1) {
        std::cout << "shmctl failed\n";
    }
}