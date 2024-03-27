创建一个线程在其添加函数，可以执行函数
```cpp
void func(int n) {
  for (int i = 1; i <= n; i++) {
    std::cout<<i<<"\n";
  }
}

int main() {
  std::thread t (func, 100);
  // t.detach();//主线程不等待子线程
  t.join();//主线程等待子线程
  std::cout<<"main\n";
}
```
如果函数的形参是引用类型需要std::ref,否则找不到重载类型
```cpp
void f(int& t, size_t n) {
  std::cout<<std::this_thread::get_id()<<'\n';
  size_t tmp{};
  for (int i = 1; i <= n; i++) {
    tmp += i;
  }
  t = tmp; 
}

int main() {
  int v{};
  std::thread t2(f, std::ref(v), 100);
  t2.join();
  std::cout<<v<<'\n';
}
```
线程创建的时候也可以用lambda,同时vector可以用来存储多个线程
```cpp
std::vector<std::thread>Threads;
  for (size_t i = 0; i < 10; i++) {
    Threads.emplace_back([=]{
      std::cout<<"this is "<<i<<"th thread\n";
    });
  }
  for (auto& i : Threads) {
    i.join();
  }
  ```