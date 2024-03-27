#include <iostream>
#include <thread>
#include <vector>

void f(int& t, size_t n) {
  size_t tmp{};
  for (int i = 1; i <= n; i++) {
    tmp += i;
  }
  t = tmp; 
}

struct X {
  void operator()() {
    std::cout<<"this is X's ()\n";
  }
};

int main() {
  std::thread t{[](){std::cout<<std::this_thread::get_id()<<'\n';}};
  t.join();

  int v{};
  std::thread t2(f, std::ref(v), 100);
  t2.join();
  std::cout<<v<<'\n';

  std::vector<std::thread>Threads;
  for (size_t i = 0; i < 10; i++) {
    Threads.emplace_back([=]{
      std::cout<<"this is "<<i<<"th thread\n";
    });
  }
  for (auto& i : Threads) {
    i.join();
  }

  // std::thread t3{ X() };
  std::thread t3 ((X()));
  t3.join();

  // using namespace std::literals;
  // std::cout<<"等待三秒自动结束\n";
  // std::this_thread::sleep_for(3s);
}