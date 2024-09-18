// 在资源管理类中小心copy行为
#include <iostream>
#include <mutex>
#include <thread>
#include <chrono>
#include <vector>
#include <memory>

class Lock {
 public:
  explicit Lock(std::mutex* pm) : mutex_ptr_(pm, [](std::mutex* m) { m->unlock(); }) { std::cout << "constructor\n", mutex_ptr_->lock(); }
  // Lock(const Lock&) = default;
  ~Lock() { std::cout <<this << " " << "destructor\n"; }
  Lock(const Lock& lock) { std::cout << &lock << " copy \n"; }
  void show() {
    std::cout << mutex_ptr_.use_count() << std::endl;
    std::cout << mutex_ptr_.get() << std::endl;
  }
 private:
  std::shared_ptr<std::mutex> mutex_ptr_;
};

void work(std::mutex* m, Lock lock) {
  std::cout << &lock << std::endl;
  Lock lock2(lock);
}

int main() {
  std::mutex m;
  Lock lock1(&m);
  // lock1.show();
  // {
  //   Lock lock2(lock1);
  //   lock2.show();
  // }
  // lock1.show();
  std::cout << &lock1 << std::endl;
  for (int i = 0 ;i < 10; i++) {
    std::thread t(work, &m, lock1);
    t.join();
  }
}