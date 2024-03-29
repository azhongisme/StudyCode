#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

template<class...Mutexs>
//命名空间不是同一个可以重名
class scoped_lock{
public:
    explicit scoped_lock(Mutexs&... Mutex) : MyMutex_(Mutex...) {
        std::lock(Mutex...);
    }
    explicit scoped_lock(std::adopt_lock_t, Mutexs&...Mutex) : MyMutex_(Mutex...) {}
    ~scoped_lock() noexcept {
        std::apply([](Mutexs&...Mutex){(...,Mutex.unlock());},MyMutex_);
    }
    scoped_lock(const scoped_lock&) = delete;
    scoped_lock& operator = (const scoped_lock&) = delete;
private:
    std::tuple<Mutexs&...> MyMutex_;
};

void f() {
    static std::mutex m1, m2;
    ::scoped_lock sl{m1, m2};
    std::cout<<111<<"\t"<<222<<"\n";
}

int main() {
    std::vector<std::thread> pool;
    for (size_t i = 0; i < 10; i++) {
        pool.emplace_back(f);
    }
    for (auto &i : pool) {
        i.join();
    }
}