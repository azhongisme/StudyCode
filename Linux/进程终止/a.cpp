#include <iostream>
#include <thread>
#include <unistd.h>

#if 1

void func1() {
    std::cout << "func1\n";
}

void func2() {
    std::cout << "func2\n";
}

int main() {
    atexit(func1);
    atexit(func2);
    return 0;
}

#endif

#if 0
class A{
public:
    A(const std::string str) : name(str) {}
    ~A() { std::cout << "destructor " << name << "\n"; }
private:
    std::string name;
};

A a1("a1");

int main() {
    A a("a2");
    _exit(0);
}
#endif

#if 0
void func() {
    std::cout << "hello world\n";
}

int main() {
    std::thread t1(func);
    t1.join();
    std::cout << "this is main func\n";
    // exit(1);
}
#endif