# 僵尸进程
如果子进程比父进程先退出，而父进程没有处理子进程退出的信息，那么子进程将成为僵尸进程
## 危害
内核为每个子进程保留一个数据结构，包括进程编号、终止状态、使用CPU时间等。父进程如果处理了子进程退出的信息，内核就会释放这个数据结构，如果没有处理，内核就不会释放，子进程的进程编号将一直被占用，系统可用的进程编号是有限的，如果产生了大量的僵尸进程，将因为没有可用的进程编号而导致系统不能产生新的进程。
```cpp
int main() {
    if (fork() == 0) {
        return 0;
    }
    while (true) {
        std::cout << "parent process is running\n";
    }
}
```
## 避免
1.子进程退出的时候，内核向父进程发出SIGCHLD信号，如果父进程用signal(SUGCHLD, SIG_IGN)通知内核表示对子进程不感兴趣

2.父进程通过wait()/waitpaid()等函数等待子进程结束，在子进程退出之前，父进程阻塞等待
```cpp
pid_t wait(int* stat_loc);
pid_t waitpid(pid_t pid, int* stat_loc, int options);
pid_t wait3()//省略参数
pid_t wait4()
```

3.如果父进程很忙，可以捕获SIGCHLD信号，在信号处理函数中调用wait()/waitpid()

