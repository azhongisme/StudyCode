```cpp
int bar_impl(int x) {
  return x;
}

typedef int(*bar)(int);
bar foo1() {
  return bar_impl;
}

auto foo2() ->int(*)(int) {
  return bar_impl;
}
```
使用传统函数声明语法的foo1无法将函数指针类型创建别名作为返回类型直接使用，而是用函数返回类型后置则没有这个问题

在C++11标准中只用decltype关键字也能写出自动推导返回类型的函数模板
```cpp
template<class T1, class T2>
decltype(T1() + T2()) sum2(T1 t1, T2 t2) {
  return t1 + t2;
}
int main() {
  sum2(4, 2);
}
```
但是这种情况要求T1和T2的默认构造函数必须存在
```cpp
class IntWrap {
 public:
  IntWrap(int n) : n_(n) {}
  IntWrap operator+ (const IntWrap& other) { return IntWrap(n_ + other.n_); }
 private:
  int n_;
};

template<class T1, class T2>
decltype(T1() + T2()) sum(T1 t1, T2 t2) {
  return t1 + t2;
}
int main() {
  sum(IntWrap(1), IntWrap(2));
}
```
这种情况就会失败，对于不想写构造的情况可以看如下示例
```cpp
template<class T1, class T2>
decltype(*static_cast<T1 *>(nullptr) + *static_cast<T2 *>(nullptr)) sum(T1 t1, T2 t2) {
  return t1 + t2;
}

template<class T>
T&& declval();

template<class T1, class T2>
decltype(declval<T1>() + declval<T2>()) sum1(T1 t1, T2 t2) {
  return t1 + t2;
}

class IntWrap {
 public:
  IntWrap(int n) : n_(n) {}
  IntWrap operator+ (const IntWrap& other) {
    std::cout << n_ + other.n_ << std::endl;
    return IntWrap(n_ + other.n_);
  }
 private:
  int n_;
};

int main() {
  sum(IntWrap(3), IntWrap(4));
  sum1(IntWrap(3), IntWrap(4));
}
```
sum使用指针类型转换和解引用求和的方法推导返回值

sum1首先通过declval将T类型转换成引用类型，这样在使用decltype推导表达式类型时不必经过构造函数检查

但是这两种都涉及了模板元编程的内容