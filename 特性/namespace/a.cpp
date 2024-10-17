#include<iostream>

namespace Parent {
  namespace Child1 {
    void foo() { std::cout << "Child1::foo()" << std::endl; }
  }
  inline namespace Child2 {
    void foo() { std::cout << "Child2::foo()" << std::endl; }
  }
}

namespace A::B::C {
  int foo() { return 5; }
}
int main() {
  Parent::Child1::foo();
  //内联命名空间能够把空间内函数和类型导出到父命名空间中
  Parent::foo();
  std::cout << A::B::C::foo() << std::endl;
}