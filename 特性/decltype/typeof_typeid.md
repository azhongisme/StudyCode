# typeof
```cpp
int a = 0;
typeof(a) b = 5;
```

# typeid
获取的类型信息会包含在一个类型为std::type_info的对象里
```cpp
  int x1 = 0;
  double x2 = 5.5;
  std::cout << typeid(x1).name() << std::endl;
  std::cout << typeid(x1 + x2).name() << std::endl;
  std::cout << typeid(int).name() << std::endl;
```
typeid的返回值是一个左值

std::type_info删除了复制构造函数，若想保存std::type_info 只能获取指针或引用
```cpp
// 编译失败，没有复制构造函数无法编译
 auto t1 = typeid(int);
// 编译成功，t2推导为const std::type_info&
  auto &t2 = typeid(int);
// 编译成功，t3推导为const std::type_info*
  auto t3 = &typeid(int);
```

typeid的返回值总是忽略类型的 cv 限定符，也就是typeid(const T)== typeid(T)