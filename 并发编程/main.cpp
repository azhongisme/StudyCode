#include <iostream>
#include <thread>
#include <mutex>


template<class Mutex>
struct lock_guard {
    explicit lock_guard(Mutex& m) : _m(m)
    {
        _m.lock();
    }
    lock_guard(const lock_guard&) = delete;
    lock_guard& operator = (const lock_guard&) = delete;

    lock_guard(Mutex&m, std::adopt_lock_t) : _m(m) {}
    ~lock_guard() {
        _m.unlock();
    }
private:
    Mutex& _m;
};

std::mutex m;

void func() {
    m.lock();
    ::lock_guard<std::mutex>lc(m, std::adopt_lock);
    std::cout<<"this is func       this is func\n";
}

int main() {
    for (int i = 0; i < 10; i++) {
        std::thread t(func);
        std::thread t1(func);
        t1.join();
        t.join();
    }
}