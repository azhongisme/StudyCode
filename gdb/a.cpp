#include <iostream>

void show() {
    std::cout << "hello world\n";
}

int main(int argc, char* argv[], char* envp) {
    std:: cout <<  "参数个数：" << argc << "\n";
    for (int i = 0; i < argc; ++i) {
        std::cout << "第" << i << "个参数： " << argv[i] << "\n";
    }
    show();

    for (int i = 0 ; i < 10; ++i) {
        std::string str = "a" + std::to_string(i);
        std::cout << str << "\n";
    }

    std::cout << "退出\n";
    return 0;
}