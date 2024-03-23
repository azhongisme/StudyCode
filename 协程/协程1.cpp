#include <iostream>
#include <coroutine>

struct promise {
  struct promise_type;

  struct iterator {
    std::coroutine_handle<promise_type>& h;
    int& operator*() {
      return h.promise().n;
    }
    iterator operator++() {
      if (!h.done())h.resume();
      return *this;
    }
    bool operator!=(const iterator&)const {
      return !h.done();
    }
  };
  struct promise_type {
    int n;
    promise get_return_object() {
      return { std::coroutine_handle<promise_type>::from_promise(*this) };
    }
    std::suspend_never initial_suspend() noexcept { return {}; }
    std::suspend_always final_suspend() noexcept { return {}; }
    std::suspend_always yield_value(int r) { n = r; return {}; }
    void return_void() {}
    void unhandled_exception() {}
  };
  iterator begin() { return {_h}; }
  iterator end() { return {_h}; }
  std::coroutine_handle<promise_type>_h;
};

promise iota(int value) {
  std::cout <<"iota\n";
  while(value) {
    co_yield value;
    --value;
  }
}

int main() {
  for (int x : iota(10)) {
    std::cout<<x<<"\n";
  }
}