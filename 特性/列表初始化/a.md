使用括号初始化的方式叫作直接初始化,使用等号初始化的方式叫作拷贝初始化（复制初始化）

# 使用列表初始化
使用大括号{}对变量进行初始化区分为直接初始化和拷贝初始化

# std::initializer_list
```cpp
class C {
 public:
  C(std::initializer_list<std::string>a) {
    for (const std::string *item = a.begin(); item != a.end(); ++item) {
      std::cout << item << " ";
    }
    std::cout << std::endl;
  }
};

int main() {
  C c{"hello" , "word"};
}
```

# 隐式缩窄转换
```cpp
int main() {
  int x = 12345;
  char y = x;
  char z{x};
}
```
从浮点类型转换整数类型。

从long double转换到double或float, 从double转换到float

从整数类型或非强枚举类型转换到浮点类型

从整数类型或非强枚举类型转换到不能代表所有原始类型值的整数类型