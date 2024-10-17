#include <iostream>
#include <assert.h>

#if 0
int main() {
  auto x1 = { 1, 2 };
  //auto x2 = { 1, 2.0 }; can't compile of different type
  //auto x3 { 1, 2 }; can't compile with over one element
  auto x4 = { 3 };
  auto x5 { 3 };
  for (auto item : x1) {
    std::cout << item << " ";
  }
  std::cout << std::endl;
}
#endif

#if 0

class Base {
 public:
  virtual void f() {
    std::cout << "Base::f()" << std::endl;
  }
};

class Derived : public Base {
 public:
  virtual void f() override {
    std::cout << "Derived::f()" << std::endl;
  }
};

int main() {
  Base* d = new Derived();
  auto b = *d;
  b.f(); //Base::f()
  auto &c = *d;
  c.f(); //Derived::f()
}

#endif

#if 0

int main() {
  auto l = [](auto a1, auto a2) { return a1 + a2; };
  auto retval = l(5, 5.0);
  std::cout << retval << std::endl;
}

#endif

#if 0

int main() {
  auto l = [](int &i)->auto& { return i; };
  auto x1 = 5;
  auto &x2 = l(x1);
  assert(&x1 == &x2);
}

#endif

#if 1

template<auto N>
void f() {
  std::cout << N << std::endl;
}

int main() {
  f<5>();
  f<'c'>();
  //f<5.0>(); double can't use as Template Parameters
}

#endif