#include <atomic>
#include <iostream>
#include <thread>
#include <vector>
 
std::atomic_int acnt;
int cnt;
 
void f()
{
    for (int n = 0; n < 1000; ++n)
    {
        ++acnt;
        ++cnt;
    }
}
 
int main()
{
    
    std::vector<std::thread> pool;
    for (int n = 0; n < 10; ++n) 
    {
        pool.emplace_back(f);
    }
    for (auto& i : pool) 
    {
        i.join();
    }
    std::cout << "原子计数器为 " << acnt << '\n'
              << "非原子计数器为 " << cnt << '\n';
}