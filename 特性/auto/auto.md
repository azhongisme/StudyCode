# 使用场景
一眼能看出声明变量的初始类型的时候可以使用

对于复杂类型，例如lambda表达式、bind等直接使用auto
```cpp
auto l = [](int a1, int a2) { return a1 + a2; };
```