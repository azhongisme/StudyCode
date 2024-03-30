#include <future>
#include <thread>
#include <iostream>

void myThread(std::promise<int>&pr, int n) {
    int t{};
    for (int i = 0; i < n; i++) {
        t += i;
    }
    pr.set_value(t);
}

int main() {
    std::promise<int>pr;
    std::thread t(myThread, std::ref(pr), 20);
    t.join();
    std::future<int>fu = pr.get_future();
    auto result =fu.get();
    std::cout<<result<<'\n';
}