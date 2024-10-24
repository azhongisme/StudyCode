#include <iostream>
#include <future>
#include <chrono>
#include <thread>

class Work {
 private:
  int value;
 public:
  Work() : value(42) {}
  std::future<int> spawn() {
    // std::this_thread::sleep_for(std::chrono::seconds(10));
    return std::async([=,  this]() ->int { return value; });
  }
};

std::future<int> foo() {
  Work tmp;
  return tmp.spawn();
}
int main() {
  std::future<int> f = foo();
  // f.wait();
  std::cout << "f.get() = " << f.get() << std::endl;
}