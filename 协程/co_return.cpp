
#include <iostream>
#include <coroutine>

struct promise;
struct coroutine : std::coroutine_handle<promise> {
  using promise_type = ::promise;
};

struct promise {
  int result{};
  coroutine get_return_object() {
    return {coroutine::from_promise(*this)};
  }
  std::suspend_never initial_suspend() noexcept { return {}; }
  std::suspend_always final_suspend() noexcept { return {}; }
  void return_value(int v) { result = v; puts("return_value 执行完毕"); }
  void unhandled_exception() {}
};

coroutine f() {
  std::cout<<"开始\n";
  co_return 100;
}

int main() {
  auto result = f();
  std::cout<<"main\n";
  std::cout<<std::boolalpha<<result.done()<<'\n';
  std::cout<<result.promise().result<<"\n";
}
