#include <iostream>

int main() {
  int a = 0;
  typeof(a) b = 5;
  int x1 = 0;
  double x2 = 5.5;
  std::cout << typeid(x1).name() << std::endl;
  std::cout << typeid(x1 + x2).name() << std::endl;
  std::cout << typeid(int).name() << std::endl;
// 编译失败，没有复制构造函数无法编译
  // auto t1 = typeid(int);
// 编译成功，t2推导为const std::type_info&
  auto &t2 = typeid(int);
// 编译成功，t3推导为const std::type_info*
  auto t3 = &typeid(int);
}