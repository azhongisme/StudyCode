#include <thread>
#include <mutex>
#include <iostream>
#include <future>
#include <charconv>

void call() {
    //没有创建线程
    std::packaged_task<int(int,int)>task([] (int a, int b) {
        std::cout<<std::this_thread::get_id()<<'\n';
        return a*b;
    });
    std::future<int>ret = task.get_future();
    task(10, 10);
    std::cout<<ret.get()<<'\n';
}

void thread_task() {
    std::packaged_task<int(int,int)>task([] (int a, int b) {
        std::cout<<std::this_thread::get_id()<<'\n';
        return a*b;
    });
    std::future<int>ret = task.get_future();
    
    std::thread t(std::move(task), 10, 10);
    t.join();
    std::cout<<ret.get()<<'\n';
}

int main() {
    std::cout<<std::this_thread::get_id()<<'\n';
    // call();
    thread_task();
}