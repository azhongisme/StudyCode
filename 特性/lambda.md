lambda类的operator()函数在默认情况下是一个const的函数，那么this指针会被const修饰
```CPP
//c++20才允许
struct X : decltype([] {std::cout<<"Hello world!";}) {

};

int main() {
  X x;
  x();
}
```
这段代码编译器看到的内容如下（没有全部粘贴）
```cpp
class __lambda_5_21
  {
    public: 
    inline /*constexpr */ void operator()() const
    {
      std::operator<<(std::cout, "Hello world!");
    }
  }
```
这就是为什么执行x()会输出hello world

# mutable
对于捕获的类型，如果需要修改就需要添加mutable
```cpp
int main() {
  // auto lam = [num = 0]()mutable{
  //   num = 1;
  // };
  // void(*p)() = [](){

  // };
  int a = 1;
  auto lam = [&a]() mutable{
    a = 2;
  };
  lam();
  std::cout<<a;
}
```
否则不允许修改

