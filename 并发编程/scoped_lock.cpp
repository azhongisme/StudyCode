#include <chrono>
#include <functional>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#if 0

std::mutex m, m1;

void f() {
    // m.lock();
    // m1.lock();
    std::lock(m, m1);
    std::cout<<"ffffff\n";
}

int main() {
    std::thread t(f);
    t.join();
}

#endif

using namespace std::chrono_literals;
 
struct Employee
{
    std::vector<std::string> lunch_partners;
    std::string id;
    std::mutex m;
    Employee(std::string id) : id(id) {}
    std::string partners() const
    {
        std::string ret = "雇员 " + id + " 的午餐伙伴: ";
        for (int count{}; const auto& partner : lunch_partners)
            ret += (count++ ? ", " : "") + partner;
        return ret;
    }
};
 
void send_mail(Employee&, Employee&)
{
    // 模拟耗时的发信操作
    std::this_thread::sleep_for(1s);
}
 
void assign_lunch_partner(Employee& e1, Employee& e2)
{
    static std::mutex io_mutex;
    {
        std::lock_guard<std::mutex> lk(io_mutex);
        std::cout << e1.id << " 和 " << e2.id << " 正等待锁定" << std::endl;
    }
 
    {
        // 用 std::scoped_lock 取得两个锁，而无需担心
        // 其他对 assign_lunch_partner 的调用死锁我们
        // 而且它亦提供便利的 RAII 风格机制
 
        std::scoped_lock lock(e1.m, e2.m);
        
        //lock_guard的可变参版本
        // 等价代码 1（用 std::lock 和 std::lock_guard ）
        // std::lock(e1.m, e2.m);
        // std::lock_guard<std::mutex> lk1(e1.m, std::adopt_lock);
        // std::lock_guard<std::mutex> lk2(e2.m, std::adopt_lock);
 
        // 等价代码 2（若需要 unique_lock，例如对于条件变量）
        // std::unique_lock<std::mutex> lk1(e1.m, std::defer_lock);
        // std::unique_lock<std::mutex> lk2(e2.m, std::defer_lock);
        // std::lock(lk1, lk2);
        {
            std::lock_guard<std::mutex> lk(io_mutex);
            std::cout << e1.id << " 和 " << e2.id << " 获得了锁" << std::endl;
        }
        e1.lunch_partners.push_back(e2.id);
        e2.lunch_partners.push_back(e1.id);
    }
 
    send_mail(e1, e2);
    send_mail(e2, e1);
}
 
int main()
{
    Employee alice("Alice"), bob("Bob"), christina("Christina"), dave("Dave");
 
    // 在并行线程中指派，因为就午餐指派发邮件消耗很长时间
    std::vector<std::thread> threads;
    threads.emplace_back(assign_lunch_partner, std::ref(alice), std::ref(bob));
    threads.emplace_back(assign_lunch_partner, std::ref(christina), std::ref(bob));
    threads.emplace_back(assign_lunch_partner, std::ref(christina), std::ref(alice));
    threads.emplace_back(assign_lunch_partner, std::ref(dave), std::ref(bob));
 
    for (auto& thread : threads)
        thread.join();
    std::cout << alice.partners() << '\n'  << bob.partners() << '\n'
              << christina.partners() << '\n' << dave.partners() << '\n';
}