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

int main() {
  auto lamda = []()->promise {
    Input t;
    std::cout<<"hello\n";
    co_await t;
    std::cout<<"end\n";
  };
  promise result = lamda();
  std::cout<<"main\n";
  result._h.resume();
}