# x++/++x
x++和++x虽然都是自增操作，但是却分为不同的左右值

x++是右值，编译器首先会生成一份x值的临时复制，然后才对x递增，最后返回临时复制内容。而++x则不同，它是直接对x递增后马上返回其自身
```cpp
int main() {
  int x = 1;
  int *p = &x++; //编译失败
  int *q = &++x; //编译成功
}
```

# 字符串字面量是一个左值
```cpp
auto p = &"hello world";
```
编译器将字符串字面量存储到程序的数据段中，程序加载的时候也会为其开辟内存空间

# 左值引用
需要将一个对象作为参数传递给子函数的时候，往往使用作之一左值引用，这样可以免去创建临时对象的操作

非常量的左值引用必须是一个左值，常量左值引用除了引用左值，还能引用右值
```cpp
int &x1 = 7;           //编译失败
const int &x = 11;     //编译成功
```
在结果上const int &x = 11和const int x = 11是一样的
```cpp
class X {
 public:
  X() {}
  X(const X&) {}
  X& operator = (const X&) { return *this; }
};

X make_x() {
  return X();
}

int main() {
  X x1;
  X x2(x1);
  X x3(make_x());
  x3 = make_x();
}
```
如果将X的复制构造函数和复制复制函数形参类型的常量性删除，则X x3(make_x());和x3 = make_x()这两句代码会编译报错

# 右值引用
```cpp
# include <iostream>

class X {
 public:
  X() { std::cout << "X ctor" << std::endl; }
  X(const X& x) { std::cout << "X copy ctor" << std::endl; }
  ~X() { std::cout << "X dtor" << std::endl; }
  void show() { std::cout << "show X" << std::endl; }
};

X make_x() {
  X x1;
  return x1;
}

int main() {
  X &&x2 = make_x();
  // X x2 = make_x();
  x2.show();
}
```

# 右值的性能优化空间
```cpp
#include <iostream>
#include <string.h>

class BigMemoryPool {
 public:
  static const int PoolSize = 4096;
  BigMemoryPool() : pool_ (new char[PoolSize]) {}
  ~BigMemoryPool() {
    if (pool_ != nullptr) {
      delete[] pool_;
    }
  }
  BigMemoryPool(const BigMemoryPool& other) : pool_(new char[PoolSize]) {
    std::cout << "copy big memory pool." << std::endl;
    memcpy(pool_, other.pool_, PoolSize);
  }
 private:
  char *pool_;
};

BigMemoryPool get_pool(const BigMemoryPool& pool) {
  return pool;
}

BigMemoryPool make_pool() {
  BigMemoryPool pool;
  return get_pool(pool);
}

int main() {
  BigMemoryPool my_pool = make_pool();
}
```
最终会输出三次copy...

get_pool返回的BigMemoryPool临时对象调用复制构造函数复制了pool对象。

make_pool返回的BigMemoryPool临时对象调用复制构造函数复制了get_pool返回的临时对象。

main函数中my_pool调用其复制构造函数复制make_pool返回的临时对象。

## 编译参数
```shell
g++ -std=c++11 -O0 -fno-elide-constructors -o b b.cpp
```

# 移动语义
```cpp
#include <iostream>
#include <string.h>

class BigMemoryPool {
 public:
  static const int PoolSize = 4096;
  BigMemoryPool() : pool_(new char[PoolSize]) {}
  ~BigMemoryPool() {
    if (pool_ != nullptr) {
      delete[] pool_;
    }
  }
  BigMemoryPool(BigMemoryPool&& other) {
    std::cout << "move big memry pool." << std::endl;
    pool_ = other.pool_;
    other.pool_ = nullptr;
  }
  BigMemoryPool(const BigMemoryPool& other) : pool_(new char[PoolSize]) {
    std::cout << "copy big memory pool." << std::endl;
    memcpy(pool_, other.pool_, PoolSize);
  }
 private:
  char *pool_;
};

BigMemoryPool get_pool(const BigMemoryPool& pool) {
  return pool;
}

BigMemoryPool make_pool() {
  BigMemoryPool pool;
  return get_pool(pool);
}

int main() {
  BigMemoryPool my_pool = make_pool();
}
```

## 输出结果
```
copy big memory pool.
move big memory pool.
move big memory pool.
```
后两次的构造函数变成了移动构造函数，对于右值编译器会优先选择使用移动构造函数去构造目标对象，当移动构造函数不存在的时候才会使用复制构造函数

除移动构造函数能实现移动语义以外，移动赋值运算符函数也能完成移动操作

## 风险
在一个移动构造函数中，如果当一个对象的资源移动到宁一个对象时发生了异常，也就是对象的一部分发生了转移而宁一部分没有，所以在编写移动语义的函数时建议确保函数不会抛出异常，如果无法保证移动移动构造函数不会抛出异常，可以使用noexcept说明符限制该函数，这样当函数抛出异常时，程序不会再继续执行下去而是调用std::terminate中止执行

# 将亡值的产生
使用类型转换将泛左值转换为该类型的右值引用
```cpp
static_cast<BigMemoryPool&&>(my_pool);
```

临时量实质化：纯右值转换到临时对象的过程（c++17）
```cpp
struct X {
  int a;
};

int main() {
  int b = X().a;
}
```
X()是一个纯右值，访问成语变量a需要一个泛左值，这里会发生一次临时量实质化，将X()转化为将亡值，最后在访问a

# 左值转换为右值
```cpp
int main() {
  int i = 0;
  int &&k = i;
}
```
上面这种情况会编译失败
```cpp
int main() {
  int i = 0;
  int &&k = static_cast<int&&>(i);
}
```
## 作用
让左值使用移动语义
```cpp
BigMemoryPool my_pool1;
BigMemoryPool my_pool2 = my_pool1;
BigMemoryPool my_pool3 = static_cast<BigMemoryPool &&>(my_pool1);
```
这种之后再次使用my_pool1会引发为定义行为，正确场景是在一个右值被转换为左值后再次转换为右值
```cpp
void move_pool(BigMemory &&pool) {
  std::cout << "call move pool" << std::endl;
  // BigMemoryPool my_pool(static_cast<BigMemoryPool&&>(pool));
  BigMemoryPool my_pool(std::move(pool));
}

int main() {
  move_pool(make_pool());
}
```

# 万能引用和引用折叠
```cpp
void foo(int &&i) {} //右值引用

template<class T>
void bar(T &&t) {} //万能引用

int getr_val() { return 5; }

int &&x = get_val(); //右值引用
auto &&y = get_val();//万能引用
```
万能引用既可以绑定左值也可以绑定右值

## 引用折叠
```
类模板型       实际类型       最终类型
T&              R           R&
T&              R&          R&
T&              R&&         R&
T&&             R           R&&
T&&             R&          R&
T&&             R&&         R&&
```
万能引用必须是T&&或者auto&&,也就是说必须在初始化的时候被直接推导出来
```cpp
#include <vector>

template<class T>
void foo(std::vector<T> &&t) {}

int main() {
  std::vector<int> v{ 1, 2, 3 };
  foo(v);
}
```
上面这段代码无法编译，因为foo的形参t并不是万能引用而是一个右值引用，是std::vector<T> &&而不是T&&

# 完美转发
```cpp
#include <iostream>
#include <string>

template<class T>
void show_type(T t) {
  std::cout << typeid(t).name() << std::endl;
}

template<class T>
void normal_forwarding(T t) {
  show_type(t);
}

int main() {
  std::string s = "hello world";
  normal_forwarding(s);
}
```
normal_forwarding是一个常规的转发函数模板，它可以完成字符串的转发任务，normal_forwarding按值转发，也就是std::string在转发过程中会额外发生一次临时对象的复制，其中一个解决办法是void nomarl_forwarding(T &t),如果传递过来的是一个右值则无法编译，虽然可以加const限定，但是无法修改

完美转发的出现改变了这个尴尬的局面
```cpp
void normal_forwarding(T&& t) {
  show_type(static_cast<T&&>(t));
}
```
之所以这里需要用到类型转换，是因为作为形参的t是左值。为了让转发将左右值页的也带到目标函数，这里需要进行类型转换。当实参是一个左值，T被推导为std::string&,当实参是一个右值被推导为std::string&&,c++提供了std::forward函数模板，不用显式转换
```cpp
template<class T>
void perfect_forwarding(T &&t) {
  show_type(std::forward<T>(t));
}
```

# 针对局部变量和右值引用的隐式移动操作
