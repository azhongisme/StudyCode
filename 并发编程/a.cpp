#include <iostream>
#include <thread>
#include <mutex>

struct X {
    mutable std::mutex m;
    void f()const {
        std::unique_lock<std::mutex>ul(m);
        ul.lock();
        std::cout<<"this is X\t"<<std::this_thread::get_id()<<'\n';
        ul.unlock();
    }
};

int main() {
    X x;
    for (int i  = 0; i < 10; i++) {
        std::thread t(&X::f, &x);
        std::thread t2(&X::f, &x);
        t.join();
        t2.join();
    }
}