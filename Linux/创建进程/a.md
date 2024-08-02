# 0、1、2进程
## 0号进程
创建1号和2号进程
## 1号进程
负责内核初始化和进行系统配置
## 2号进程
负责所有内核线程的调度和管理

# 查看进程树
```shell
pstree -p 进程编号
```

# 进程标识
每个进程都有一个非负整数表示的唯一的进程ID
```cpp
pid_t getpid(void); //获取当前进程的ID
pid_t getppid(void); //获取父进程的ID
```
# fork()函数
创建一个新的进程,父进程和子进程从fork()之后的代码开始执行,父进程返回子进程的pid，子进程返回0
```cpp
pid_t fork(void);
```
```cpp
#include <iostream>
#include <unistd.h>

int main() {
    std::string str = "hello world\n";

    fork();
    
    std::cout << str;
}
```
这段代码会输出两个hello world

子进程是父进程的副本，子进程获得父进程数据空间、堆和栈的副本(不是共享)(虚拟地址相同，但实际物理地址不同)
## 用法
1） 父进程复制自己，然后父进程和子进程执行不同的代码。如：父进程等待客户端的链接请求，当请求到达时，父进程调用fork()，让子进程处理这些请求，而父进程则等待下一个连接请求
```cpp
    pid_t pid = fork();

    if(pid > 0) {
        std::cout << "this is parent process\n";
    } else {
        std::cout << "this is children process\n";
    }
```

2） 进程执行宁一个程序。 子进程从fork返回后立即调用exec
```cpp
int main() {
    if (fork() > 0) {
        while(true) {
            sleep(1);
            std::cout << "parent process\n";
        }
    } else {
        std::cout << "children process\n";
        execl("/bin/ls", "ls", ".", nullptr);
    }
}
```

## 共享文件
父进程和子进程共享一个文件偏移量
```cpp
#include <iostream>
#include <unistd.h>
#include <fstream>

#if 1
int main() {
    std::ofstream fout;
    fout.open("a.txt");
    if(!fout) {
        return -1;
    }

    fork();

    for (int i = 0; i < 10000000; ++i) {
        fout << i << "\n";
    }
    fout.close();
}
```
这段代码最终会向a.txt中写入20000000行数据，如果没有fork将程序挂在后台两个同时跑最终只有10000000行数据
```shell
./a &
./a &
```
如果父进程和子进程写同一描述符指向的文件，但没有任何形式的同步，那么输出可能会相互混合

# vfork
创建一个新的进程，而新的进程目的是exec一个新程序，**vfork**保证子进程先运行,在子进程调用exec或exit后父进程才恢复
```cpp
int main() {
    pid_t pid = vfork();
    if (pid > 0) {
        std::cout << "parent process\n";
    } else {
        sleep(10);
        execl("/bin/ls", "ls", ".", nullptr);
    }
}
```