#include<iostream>
#include<thread>

#if 0
void func() {
  std::cout<<"hello world\n";
}

int main() {
  //调用func
  std:: thread t(func);
  t.join();
}
#endif

#if 0

int main() {
  std::thread t ([] {
    std::cout<<"hello world\n";
  });
  t.join();
}
#endif

#if 0
void func(std::string str) {
  std::cout<<str<<'\n';
}

int main() {
  std::thread t (func, "hello world");
  t.join();
}

#endif

void func(int n) {
  for (int i = 1; i <= n; i++) {
    std::cout<<i<<"\n";
  }
}

int main() {
  std::thread t (func, 100);
  t.detach();//主线程不等待子线程
  std::cout<<"main\n";
}