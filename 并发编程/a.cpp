#include<iostream>
#include<thread>

void func() {
  std::cout<<"hello world\n";
}

int main() {
  std:: thread t(func);
  t.join();
}