#include <mutex>
#include <thread>
#include <iostream>
#include <condition_variable>

std::mutex mtx;
std::condition_variable cv;
bool ready = false;

void do_print_id(int id) {
    std::unique_lock <std::mutex> lck(mtx);
    cv.wait(lck, [] {return ready == true; }); //第二个是参数为false那么将阻塞，参数需要函数be used as a function
    std::cout<<"thread"<<id<<'\n';
}
void go() {
    std::lock_guard<std::mutex> lck(mtx);
    ready = true;
    //cv.notify_one(); 唤醒一个线程
    cv.notify_all(); //唤醒所有线程
}

int main() {
    std::thread threads[10];
    for (int i = 0; i < 10; i++) {
        threads[i] = std::thread(do_print_id, i);
    }
    std::cout<<"10 threads ready to race...\n";
    go();
    for (auto& th : threads) {
        th.join();
    }
}
