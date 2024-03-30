#include <thread>
#include <mutex>
#include <iostream>
#include <future>
#include <charconv>

// int f(int n) {
//     int t{};
//     for (int i = 0; i < n; i++) {
//         t += i;
//     }
//     using namespace std::chrono_literals;
//     std::this_thread::sleep_for(3s);
//     return t;
// }

//最常用的成员函数get(),wait()
//get()要求线程必须执行完毕拿到返回值结果,如果线程没有完成就会一直等待
//wait()用于等待线程返回，本身并不返回结果

// int main() {
//     std::future<int> ret = std::async(std::launch::async | std::launch::async, f, 10);
//     std::cout<<ret.get()<<"\n";
// }
 
using namespace std::chrono_literals;
 
int main()
{
    std::future<int> future = std::async(std::launch::async, []()
    {
        std::this_thread::sleep_for(3s);
        return 8;
    });
 
    std::cout << "等待...\n";
    std::future_status status;
 
    do
    {
        switch (status = future.wait_for(1s); status)
        {
            case std::future_status::deferred:
                std::cout << "延后\n";
                break;
            case std::future_status::timeout:
                std::cout << "超时\n";
                break;
            case std::future_status::ready:
                std::cout << "就绪!\n";
                break;
        }
    }
    while (status != std::future_status::ready);
 
    std::cout << "结果为 " << future.get() << '\n';
}