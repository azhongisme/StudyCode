#include <mutex>
#include <thread>
#include <iostream>
#include <vector>

int n{};
std::mutex m;
std::once_flag fl;

void f() {
    std::lock_guard<std::mutex>lc(m);
    n++;
    std::cout<<n<<"\n";
    throw std::runtime_error("");
}

void f2() {
    std::lock_guard<std::mutex>lc(m);
    n++;
    std::cout<<n<<"\n";
}

int main() {
    // std::call_once(fl, f);
    // std::call_once(fl, f);
    // std::call_once(fl, f);
    // std::call_once(fl, f);
    // try
    // {
    //     std::call_once(fl, f);
    // }
    // catch (const std::exception&) 
    // {

    // }
    // try
    // {
    //     std::call_once(fl, f);
    // }
    // catch (const std::exception&) 
    // {

    // }
    std::vector<std::thread>v;
    v.emplace_back([](){std::call_once(fl, f2);});
    v.emplace_back([](){std::call_once(fl, f2);});
    v.emplace_back([](){std::call_once(fl, f2);});
    v.emplace_back([](){std::call_once(fl, f2);});
    for (auto& i : v) {
        i.join();
    }
}