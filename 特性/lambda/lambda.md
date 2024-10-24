```cpp
[ captures ] ( params ) specifiers exception -> ret { body }
```
[ captures ] —— 捕获列表，它可以捕获当前函数作用域的零个或多个变量，变量之间用逗号分隔

捕获列表的捕获方式有两种：按值捕获和引用捕获

specifiers —— 可选限定符，C++11中可以用mutable，它允许
我们在lambda表达式函数体内改变按值捕获的变量，或者调用非const
的成员函数。

exception —— 可选异常说明符，我们可以使用noexcept来指
明lambda是否会抛出异常

ret —— 可选返回值类型,lambda表达式使用返回类型后置的语法来表示返回类型

# 捕获
```cpp
#include <iostream>
int main() {
  int x = 5, y = 8;
  auto foo = [x, &y]() mutable {
    x += 1;
    y += 2;
    std::cout << "lambda x = " << x << ", y = " << y << std::endl;
    return x * y;
  };
  foo();
  std::cout << "call1 x = " << x << ", y = " << y << std::endl;
  foo();
  std::cout << "call2 x = " << x << ", y = " << y << std::endl;
}
```
输出结果
```
lambda x = 6, y = 10
call1 x = 5, y = 10
lambda x = 7, y = 12
call2 x = 5, y = 12
```
外部作用域的x不变，但是影响下次调用lambda中x的值

捕获值的变量在
lambda表达式定义的时候已经固定下来了，无论函数在lambda表达式
定义后如何修改外部变量的值，lambda表达式捕获的值都不会变化
```cpp
#include <iostream>
int main() {
  int x = 5, y = 8;
  auto foo = [x, &y]() mutable {
    x += 1;
    y += 2;
    std::cout << "lambda x = " << x << ", y = " << y << std::endl;
    return x * y;
  };
  x = 9;
  y = 20;
  foo();
}
```
# 特殊捕获
```cpp
[this]  捕获this指针
[=] 捕获lambda表达式定义作用域的全部变量的值，包括this
[&] 捕获lambda表达式定义作用域的全部变量的引用，包括this
```

```cpp
#include <iostream>

class A {
 public:
  void print() {
    std::cout << "class A" << std::endl;
  }

  void test() {
    auto foo = [this] {
      print();
      x = 5;
    };
    foo();
  }
 private:
  int x;
};

int main() {
  A a;
  a.test();
}
```

# 无状态lambda表达式
无状态lambda是指没有捕获

无状态lambda可以隐式转换为函数指针
```cpp
void f(void(*) ()) {}
void g() { f([] {}); }
```
```cpp
void f(void(&)()) {}
void g() { f(*[] {}); }
```

# STL中使用lambda
直接在STL算法函数的参数列表内实现辅助函数
```cpp
#include <iostream>
#include <vector>
#include <algorithm>

int main() {
  std::vector<int> x = { 1, 2, 3, 4, 5 };
  std::cout << *std::find_if(x.cbegin(), x.cend(), [](int i) { return (i % 3) == 0; }) << std::endl;
}std::map<std::string, int, decltype(greater)> mymap; 

  int x = 5;
  auto foo = [x = x + 1] { return x; };
}
```
这种在c++14之前无法编译通过。这段代码通过=跨越了两个作用域，左边x存在与lambda表达式作用域，右边x存在于main函数作用域
。下面这种写法更清晰
```cpp
int main() {
  int x = 5;
  auto foo = [r = x + 1] { return x; };
}
下面举两个例子
```cpp
#include <iostream>

int main() {
  std::string x = "hello";
  auto foo = [x = std::move(x)] { return x + "word\n"; };
  std::cout << foo();
}
```
这个场景是使用移动操作减少代码运行的开销
```cpp
#include <iostream>
#include <future>
#include <chrono>
#include <thread>

class Work {
 private:
  int value;
 public:
  Work() : value(42) {}
  std::future<int> spawn() {
    // std::this_thread::sleep_for(std::chrono::seconds(10));
    return std::async([=]() ->int { return value; });
  }
};

std::future<int> foo() {
  Work tmp;
  return tmp.spawn();
}
int main() {
  std::future<int> f = foo();
  // f.wait();
  std::cout << "f.get() = " << f.get() << std::endl;
}
```
这个场景在异步调用时对象被销毁导致未定义行为
```cpp
  std::future<int> spawn() {
    // std::this_thread::sleep_for(std::chrono::seconds(10));
    return std::async([=, tmp = *this]() ->int { return tmp.value; });
  }
```
这里只需要这样修改就好

# 泛型lambda表达式
C++14标准让lambda表达式具备了模版函数的能力,但是它的定义方式却用不到template关键字。
```cpp
int main() {
  auto foo = [](auto a) { return a; };
  int three = foo(3);
  char const* hello = foo("hello");
}
```

# 常量lambda表达式和捕获*this
c++17标准对lambda表达式同样有两处增强，一处是常量lambda表达式，另一处是对捕获*this的增强。
其中常量lambda表达式的主要特性体现在constexpr关键字上

这里主要说捕获*this
```cpp
class Work {
 private:
  int value;
 public:
  Work() : value(42) {}
  std::future<int> spawn() {
    // std::this_thread::sleep_for(std::chrono::seconds(10));
    return std::async([=,  *this]() ->int { return value; });
  }
};
```
[*this]的语法让程序生成了一个*this对象的副本并存储在lambda表
达式内，可以在lambda表达式内直接访问这个复制对象的成员

# 捕获[=, this]
[=]可以捕获this指针,[=,*this]会捕获this对象的副本

在C++20标准中引入了[=, this]捕获this指针的语法，它实际上表达的意思和[=]相同，目的是
让程序员们区分它与[=,*this]的不同

在C++20标准中还特别强调了要用[=, this]代替[=]
```cpp
template<class T>
void g(T) {}

struct Foo {
  int n = 0;
  void f(int a) {
    g([=](int k) { return n + a * k; });
  }
};
```
```
 warning: implicit capture of ‘this’ via ‘[=]’ is deprecated in C++20 [-Wdeprecated]
    7 |     g([=](int k) { return n + a * k; });
      |       ^
h.cpp:7:7: note: add explicit ‘this’ or ‘*this’ capture
/usr/bin/ld: cannot use executable file 'h' as input to a link
collect2: error: ld returned 1 exit status
```

最后值得注意的是，同时用两种语法捕获[this, *this] {};是不允许的

# 模板语法的泛型lambda表达式
C++14标准中lambda表达式通过支持auto来实现泛型。
但不幸的是，这种语法也会使我们难以与类型进行互动，对类型的操作变得异常复杂。
```cpp
template <typename T> struct is_std_vector : std::false_type {};
template <typename T> struct is_std_vector<std::vector<T>> : std::true_type {};
auto f = [](auto vector) {
  static_assert(is_std_vector<decltype(vector)>::value, "");
};
```
普通的函数模板可以轻松地通过形参模式匹配一个实参为vector的容器对象，但是对于lambda表达式，auto不具备这种表达能力，所以
不得不实现is_std_vector，并且通过static_assert来辅助判断实参的真实类型是否为vector。

除此之外，这样的语法让获取vector存储对象的类型也变得十分复杂
```cpp
auto f = [](auto vector) {
  using T = typename decltype(vector)::value_type;
};
```
这种只是内嵌类型value_type表示存储对象的类型，decltype(obj)有时候并不能直接获取我们想要的类型。
```cpp
#include <vector>

auto f = [](const auto& x) {
  using T = decltype(x);                 //可以编译，但是语义错误
  T copy = x;
  using Iterator = typename T::iterator; //编译错误
};

std::vector<int> v;

int main() {
  f(v);
}
```
decltype(x)推导出来的类型是const std::vector &，所以T copy = x;不是一个复制而是引用。对于一个引用类型来说，T::iterator也是不符合语法的，所以
编译出错。

```cpp
[]<typename T>(T t) {}
```
鉴于以上种种问题，C++委员会决定在C++20中添加模板对lambda的支持
```cpp
auto f = []<typename T>(std::vector<T> vector) {
  //...
};
```
```cpp
auto f = []<typename T>(T const& x) {
  T copy = x;
  using Iterator = typename T::iterator;
};
```

# 可构造和可赋值的无状态lambda(c++20)
像std::sort和std::find_if这样的函数需要一个函数对象或函数指针来
辅助排序和查找，这种情况我们可以使用lambda表达式完成任务。但
是如果遇到std::map这种容器类型就不好办了，因为std::map的比较函
数对象是通过模板参数确定的
```cpp
auto greater = [](auto x, auto y) { return x > y; };
std::map<std::string, int, decltype(greater)> mymap; 
```
