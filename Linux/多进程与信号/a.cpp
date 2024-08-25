#include <iostream>
#include <unistd.h>
#include <signal.h>

void FathEXIT(int sig);
void ChildEXIT(int sig);

int main() {
    for (int i = 0 ; i <= 64; ++i) {
        signal(i, SIG_IGN);
    }
    signal(SIGTERM, FathEXIT);
    signal(SIGINT, FathEXIT);

    while (true) {
        if (fork() > 0) {
            sleep(5);
            continue;
        } else {
            signal(SIGTERM, ChildEXIT);
            signal(SIGINT, SIG_IGN);

            while (true) {
                std::cout << "child process " << getpid() << " is running\n";
                sleep(3);
                continue;
            }
        }
    }
}

void FathEXIT(int sig) {
    signal(SIGINT, SIG_IGN);
    signal(SIGTERM, SIG_IGN);

    std::cout << "father process exit, sig = " << sig << std::endl;
    kill(0, SIGTERM);

    //实际开发中添加释放资源的代码
    exit(0);
}

void ChildEXIT(int sig) {
    signal(SIGINT, SIG_IGN);
    signal(SIGTERM, SIG_IGN);

    std::cout << "child process " << getpid() << " exit, sig = " << sig << std::endl;
    //释放子进程资源
    exit(0);
}