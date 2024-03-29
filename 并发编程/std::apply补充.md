在实现中的析构函数
```cpp
~scoped_lock() noexcept {
    std::apply([](Mutexs&...Mutex){(...,Mutex.unlock());},MyMutex_);
}
```
std::apply 是一个函数模板，它将一个函数应用于元组的每个元素。

它可以用来展开一个 std::tuple 或者一个 std::pair 的元素，并将它们作为参数传递给一个可调用对象



