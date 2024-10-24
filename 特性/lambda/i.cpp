#include <vector>
auto f = []<typename T>(std::vector<T> vector) {
  //...
};

auto f = []<typename T>(T const& x) {
  T copy = x;
  using Iterator = typename T::iterator;
};