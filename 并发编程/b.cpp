#include <thread>
#include <iostream>

class thread_guard {
  std::thread& t_;
public:
  explicit thread_guard(std::thread& t) : t_(t) {}
  ~thread_guard() {
    if (t_.joinable()) {
      t_.join();
    }
  }
  thread_guard(thread_guard const&) = delete;
  thread_guard& operator = (thread_guard const&) = delete;
};

void f(int& i) {
  //n在call函数执行完后会被销毁，那么这个引用就悬空了，引用已经被销毁的地址，就会造成未定义行为
  i++;
}

void call() {
  int n{};
  std::thread t(f, std::ref(n));
  ::thread_guard t2 (t);
}

int main() {
  call();
}