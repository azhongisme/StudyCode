#include <iostream>
#include <coroutine>

struct promise {
  struct promise_type {
    promise get_return_object() {
      return { std::coroutine_handle<promise_type>::from_promise(*this) };
    }
    std::suspend_never initial_suspend() noexcept { return {}; }
    std::suspend_always final_suspend() noexcept { return {}; }
    void return_void() {}
    void unhandled_exception() {}
  };
  std::coroutine_handle<promise_type>_h;
};
struct Input {
  bool await_ready() {
    return false;
  }
  void await_suspend(std::coroutine_handle<>h) {}
  void await_resume() {}
};

promise func() {
  Input t;
  std::cout<<"协程开始\n";
  co_await t;
  std::cout<<"协程结束\n";
}

int main() {
  promise result = func();
  std::cout<<"main\n";
  result._h.resume();
}