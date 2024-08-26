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