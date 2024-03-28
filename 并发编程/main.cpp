#include <iostream>
#include <thread>
#include <mutex>

std::mutex m;

void func() {
    m.lock();
    std::lock_guard<std::mutex>lc(m, std::adopt_lock);
    std::cout<<"hello world\n";
}

int main() {
    std::thread t(func);
    t.join();
}