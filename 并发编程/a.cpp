#include<iostream>
#include<thread>

void func() {
  std::cout<<"hello world\n";
}

int main() {
  //调用func
  std:: thread t(func);
  t.join();
}