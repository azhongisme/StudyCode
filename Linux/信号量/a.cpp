//给共享内存加锁
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