# 共享内存
多线程共享进程的地址空间，如果多个线程需要访问同一块内存，用全局变量就可以了

在多进程中，每个进程的地址空间是独立的，不共享的，如果多个进程需要访问同一块内存，只能用共享内存

共享内存没有提供锁机制，如果要对共享内存的读/写加锁，可以使用信号量

# shmget
该函数用于创建/获取共享内存
```cpp
int shmget(key_t key, size_t size, int shmflg);
//key 共享内存的键值，是一个整数(typedef unsinged int key_t)
//不能共享的key不同
//共享内存的大小
//shmflg共享内存的访问权限
//返回值 共享内存的id， 失败返回-1（系统内存不足，没有权限）
```

# 查看共享内存
```shell
$ ipcs -m
```

# 删除共享内存
```shell
$ ipcrm -m 编号(shmid)
```

# shmat
该函数用于把共享内存连接到当前进程的地址空间
```cpp
void *shmat(int shmid, const void *shmaddr, int shmflg);
//shmaddr 指定共享内存连接到当前进程中的地址位置，通常填0,表示让操作系统来选择共享内存的地址
//shmflg 标志位，通常填0
//调用成功返回共享内存起始地址，失败返回（void*）-1
```
# 注意
共享内存不能使用stl容器，因为stl会在堆区动态分配内存


```cpp
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
```