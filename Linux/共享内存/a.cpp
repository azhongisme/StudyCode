#include <sys/ipc.h>
#include <sys/shm.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>

struct stgirl {
    int no;
    char name[51];
};

int main(int argc, char* argv[]) {
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

    std::cout << "原值： no = " << ptr->no << ", name = " << ptr->name << std::endl;
    ptr->no = atoi(argv[1]);
    strcpy(ptr->name, argv[2]);
    std::cout << "新值： no = " << ptr->no << ", name = " << ptr->name << std::endl;

    //把共享内存从当前进程分离
    shmdt(ptr);

    //删除共享内存
    if (shmctl(shmid, IPC_RMID, 0) == -1) {
        std::cout << "shmctl failed\n";
    }
}