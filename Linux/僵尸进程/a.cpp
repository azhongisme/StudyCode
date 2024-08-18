#include <sys/wait.h>
#include <sys/types.h>
#include <iostream>

#if 1
void func(int sig) {
    int sts;
    pid_t pid = wait(&sts);
    std::cout << "the ending process id is: " << pid << "\n";
    if (WIFEXITED(sts)) {
        std::cout << "child process is correct exit, the eixt status is: " << WEXITSTATUS(sts) << "\n";
    } else {
        std::cout << "child process is wrong in exit, the signal to terminate is: " << WTERMSIG(sts) << "\n";
    }
}

int main() {
    signal(SIGCHLD, func);
    if (fork() > 0) {
        while(true) {
            std::cout << "parent process is busy\n";
            sleep(1);
        }
    } else {
        sleep(5);
        exit(0);
    }
}

#endif

#if 0
int main() {
    if (fork() > 0) {
        int sts;
        pid_t pid = wait(&sts);
        std::cout << "the ending process id is: " << pid << "\n";
        if (WIFEXITED(sts)) {
            std::cout << "child process is correct exit, the eixt status is: " << WEXITSTATUS(sts) << "\n";
        } else {
            std::cout << "child process is wrong in exit, the signal to terminate is: " << WTERMSIG(sts) << "\n";
        }
    }
}
#endif 

#if 0
int main() {
    // if (fork() > 0) {
    //     sleep(20);
    //     std::cout << "parent  process is out\n";
    //     return 0;
    // } else {
    //     for (int i = 0 ; i < 100; i++) {
    //         std::cout << "child process is running\n";
    //         sleep(1);
    //     }
    // }
    signal(SIGCHLD, SIG_IGN);
    if (fork() == 0) {
        return 0;
    }
    while (true) {
        std::cout << "parent process is running\n";
        sleep(1);
    }
    waitid
}
#endif