#include <iostream>
#include <assert.h>

template<class T>
T sum(T a1, T a2) {
  return a1 + a2;
}

template<class R, class T1, class T2>
R sum(T1 a1, T2 a2) {
  return a1 + a2;
}

template<class T1, class T2>
auto sum(T1 a1, T2 a2) ->decltype(a1 + a2) {
  return a1 + a2;
}

template<class T>
auto return_ref(T& t) ->decltype(t) {
  return t;
}

int main() {
  int x1 = 0;
  static_assert(std::is_reference_v<decltype(return_ref(x1))>);
}