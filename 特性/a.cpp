#include <iostream>

void func(int && a) {
  printf(__PRETTY_FUNCTION__);
  std::cout<<std::endl;
}

void func(int &a) {
  printf(__PRETTY_FUNCTION__);
  std::cout<<std::endl;
}

int main() {
  func(1);//prvalue
  int a = 2;
  func(a);//xvalue
  func(std::move(a));//lvalue
}