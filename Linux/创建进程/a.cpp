#include <iostream>
#include <unistd.h>
#include <fstream>

#if 1
int main() {
    pid_t pid = vfork();
    if (pid > 0) {
        std::cout << "parent process\n";
    } else {
        sleep(10);
        execl("/bin/ls", "ls", ".", nullptr);
    }
}

#endif


#if 0
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

#endif

#if 0
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

#endif

#if 0
int main() {
    pid_t pid = fork();

    if(pid > 0) {
        std::cout << "this is parent process\n";
    } else {
        std::cout << "this is children process\n";
    }
}
#endif