#include <iostream>

class IntWrap {
 public:
  IntWrap(int n) : n_(n) {}
  IntWrap operator+ (const IntWrap& other) { return IntWrap(n_ + other.n_); }
 private:
  int n_;
};

template<class T1, class T2>
decltype(T1() + T2()) sum(T1 t1, T2 t2) {
  return t1 + t2;
}
int main() {
  sum(IntWrap(1), IntWrap(2));
}