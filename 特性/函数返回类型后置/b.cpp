#include <iostream>

template<class T1, class T2>
decltype(*static_cast<T1 *>(nullptr) + *static_cast<T2 *>(nullptr)) sum(T1 t1, T2 t2) {
  return t1 + t2;
}

template<class T>
T&& declval();

template<class T1, class T2>
decltype(declval<T1>() + declval<T2>()) sum1(T1 t1, T2 t2) {
  return t1 + t2;
}

class IntWrap {
 public:
  IntWrap(int n) : n_(n) {}
  IntWrap operator+ (const IntWrap& other) {
    std::cout << n_ + other.n_ << std::endl;
    return IntWrap(n_ + other.n_);
  }
 private:
  int n_;
};

int main() {
  sum(IntWrap(3), IntWrap(4));
  sum1(IntWrap(3), IntWrap(4));
}