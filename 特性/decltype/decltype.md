```cpp
  int x1 = 0;
  decltype(x1) x2 = 0;
  std::cout << typeid(x2).name() << std::endl;

  double x3 = 0;
  decltype(x1 + x3) x4 = x1 + x3;
  std::cout << typeid(x4).name() << std::endl;
```

```cpp
int x1 = 0;
decltype(x1) sum(decltype(x1) a1, decltype(a1) a2) {
  return a1 + a2;
}

int main() {
  auto x2 = sum(5, 10);
}
```

```template<class T>
T sum(T a1, T a2) {
  return a1 + a2;
}

int main() {
  auto x1 = sum(5, 10.0);
}
```
上述情况无法通过编译
```cpp
template<class R, class T1, class T2>
R sum(T1 a1, T2 a2) {
  return a1 + a2;
}

int main() {
  auto x1 = sum<double>(5, 10.0);
}
```
但这种情况必须为函数模板指定返回值
```cpp
template<class T1, class T2>
auto sum(T1 a1, T2 a2) ->decltype(a1 + a2) {
  return a1 + a2;
}
```
上述情况只适合c++11标准，因为14支持对auto声明的返回类型进行推导了
```cpp
template<class T1, class T2> 
auto sum(T1 a1, T2 a2) {
  return a1 + a2;
}

```cpp
template<class T>
auto return_ref(T& t){
  return t;
}

int main() {
  int x1 = 0;
  static_assert(std::is_reference_v<decltype(return_ref(x1))>);
}
```
静态断言失败
```cpp
template<class T>
auto return_ref(T& t) ->decltype(t) {
  return t;
}

int main() {
  int x1 = 0;
  static_assert(std::is_reference_v<decltype(return_ref(x1))>);
}
```

# 推导规则
decltype(e)其中e的类型为T

1.如果e是一个未加括号的标识符表达式（结构化绑定除外）或者未加括号的类成员访问，则decltype（e）的类型是T。如果不存在这样的类型或e是一组重载函数，无法推导

2.如果e是一个函数调用或者仿函数调用，decltype（e）推断出的类型是返回值

3.如果e的类型是一个T的左值，推导为T&

4.如果e是一个将亡值，推导为T&&

5.除以上情况，推导为T

```cpp
const int&& foo();
int i;
struct A {
double x;
};
const A* a = new A();
decltype(foo()); //const int&&
decltype(i);     //int
decltype(a->x);  //double
decltype((a->x));//const double&
```
# decltype(auto)