#include <iostream>
#include <coroutine>
#include <thread>
#include <chrono>

struct promise;
struct coroutine : std::coroutine_handle<promise> {
  using promise_type = ::promise;
};//coroutine继承std::coroutine_handle<promise>

struct promise {
  int n;
  coroutine get_return_object() {
    return { coroutine::from_promise(*this) };
  }
  std::suspend_never initial_suspend() noexcept { return {}; }
  std::suspend_always final_suspend() noexcept { return {}; }
  std::suspend_always yield_value(int r) { n = r; return {}; }
  void return_void() {}
  void unhandled_exception() {}
};

template<typename T>
struct Future {
  T n;
  bool await_ready() {
    return false;
  }
  //协程暂停后调用await_suspend
  void await_suspend(std::coroutine_handle<>h) {
    std::thread thread{[this,h] {
      int t = this->n;
      for (int i = 1; i < t; i++) {
        this-> n *= i;
      }
      if (!h.done()) h.resume();
    }};
    thread.detach();
  }
  T await_resume() { return n; }
};

template<typename T>
inline auto operator co_await(const Future<T>& x) {
  std::cout<<"operator co_await 被调用 X = "<<x.n<<'\n';
  return x;
}

coroutine f() {
  Future fu{ 5 }; 
  int v = co_await fu;
  std::cout<<"n: "<<v<<'\n';
  co_yield 20;
}

int main() {
  auto result = f();
  std::cout<<"main\n";
  using namespace std::literals;
  std::this_thread::sleep_for(100ms);
  result.resume();
  std::cout<<std::boolalpha<<result.done()<<'\n';
  std::cout<<"co_yield value: "<<result.promise().n<<'\n';
}