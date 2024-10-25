```cpp
class X {
 public:
  X() : a_(0), b_(0.), c_("hello world") {}
  X(int a) : a_(a), b_(0.), c_("hello world") {}
  X(double b) : a_(0), b_(b), c_("hello world") {}
  X(const std::string& c) : a_(0), b_(0.), c_(c) {} 
 private:
  int a_;
  double b_;
  std::string c_;
};
```
在上面的代码中，X有四个构造函数，为了在构造的时候初始化非静态数据成员，它们的初始化列表有一些冗余代码。
C++11标准提出了新的初始化方法，即在声明非静态数据成员的同时直接对其使用=或者{}
```cpp
class X {
 public:
  X() {}
  X(int a) : a_(a) {}
  X(double b) b_(b) {}
  X(const std::string &c) : c_(c) {}
 
 private:
  int a_ = 0;
  double b { 0. };
  std::string c_{ "hello world" }; 
};
```
# 位域的默认初始化
```cpp
struct S {
  int y : 8 = 11;  //y占用8位
  int z : 4 { 7 }; //z占用4位
};
```

## 注意当表示位域的常量表达式是一个条件表达式时
```cpp
int a;
struct S2 {
  int y : true ? 8 : a = 42;
  int z : 1 || new int { 0 };
};
```
这段代码中并不存在默认初始化
```cpp
struct S3 {
  int y : (true ? 8 : a) = 42;
  int z : (1 || new int) {0};
};
```