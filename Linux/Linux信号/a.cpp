#include <iostream>
#include <unistd.h>
#include <signal.h>

#if 1
void EXIT(int signum) {
    std::cout << "收到了信号： " << signum << "\n";
    std::cout << "正在释放资源，程序将退出.....\n";

    std::cout << "程序退出\n";
    exit(0);
}

int main() {
    //防止被其他信号异常终止
    for (int i = 1; i <= 64; ++i) {
        signal(i, SIG_IGN); //忽略所有信号
    }
    signal(2, EXIT);
    signal(15, EXIT);

    while(true) {
        std::cout << "后台正在运行\n";
        sleep(1);
    }
}
#endif

#if 0

void func(int signum) {
    std::cout << "收到了信号： " << signum << "\n";
    signal(signum, SIG_DFL); //恢复信号的处理方法为默认的行为， 当第二次发送1的信号才会挂起，第一次仍是执行func函数
}

int main(int argc, char* argv[]) {
    signal(1,func); // 如果接受到1的信号，那么执行func
    signal(15,func);
    signal(2,SIG_IGN); //忽略2的信号
    signal(9, func); //无效， 信号9不能被捕获也不能被忽略，强制杀死程序
    signal(9, SIG_IGN);
    signal(14, func);

    alarm(5); //5s后向本程序发送14的信号
    while(true) {
        std::cout << "执行一次任务\n";
        sleep(1);
    }
}

#endif