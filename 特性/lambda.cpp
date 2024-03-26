#include <iostream>

#if 0
//c++20才允许
struct X : decltype([] {std::cout<<"Hello world!";}) {

};

int main() {
  X x;
  x();
}
#endif

int main() {
  // auto lam = [num = 0]()mutable{
  //   num = 1;
  // };
  // void(*p)() = [](){

  // };
  int a = 1;
  auto lam = [a]()mutable {
    a = 2;
  };
  lam();
  std::cout<<a<<'\n';
}
