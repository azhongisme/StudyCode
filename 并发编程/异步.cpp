#include <thread>
#include <mutex>
#include <iostream>
#include <future>

constexpr int f(int n) {
    int t{};
    for (int i = 0; i < n; i++) {
        t += i;
    }
    return t;
}

//std::launch::deferred 延迟调用，不会在线程里面运行，延迟到get(),wait()执行
//std::launch::async在线程里面执行
//默认情况下std::launch::async
//std::launch::async | std::launch::async表示是否创建线程由系统调度决定，如果资源紧张就不会创建线程
int main() {
    // std::future<int>ret = std::async(f, 10);
    std::future<int>ret = std::async(std::launch::async | std::launch::async, f, 10);
    std::cout<<ret.get();
}