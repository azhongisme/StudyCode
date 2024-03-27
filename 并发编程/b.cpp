#include <thread>
#include <iostream>

auto f() {
  std::thread t{[](int n) {
    std::cout<<n<<'\n';
  }, 10};
  return t;
}

int main() {
  std::thread t = f();
  std::thread t2 = std::move(t);
  //t.join(); 非法，线程所有权移动到了t2
  t2.join();
}