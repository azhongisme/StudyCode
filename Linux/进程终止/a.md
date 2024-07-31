# 进程终止
有8种方式，其中5种为正常终止
1. 在main()函数用return 返回
2. 在任意函数中调用exit()函数
3. 在任意函数中调用_exit()和_Exit()函数
4. 最后一个线程从其启动例程(线程主函数)用return返回
5. 在最后一个线程中调用pthread_exit()函数
异常终止有3种
6. 调用abort()
7. 接收到一个信号
8. 最后一个线程对取消请求做出响应

# 进程终止状态
在main()中，return的返回值即终止状态，如果没有return语句或调用exit()，那么终止状态是0

在shell中查看终止状态
```shell
echo $？
```

# 资源释放
return表示函数返回，会调用局部对象的析构函数，main()函数中的return还会调用全局对象的析构函数

exit()表示终止进程，不会调用局部对象的析构函数，只调用全局对象的析构函数

exit()会执行清理工作，然后退出，_exit()和_Exit()直接退出，不执行清理工作

# atexit
进程可以用atexit()函数登记终止函数，调用终止函数的顺序与登记的相反
```cpp
void func1() {
    std::cout << "func1\n";
}

void func2() {
    std::cout << "func2\n";
}

int main() {
    atexit(func1);
    atexit(func2);
    return 0;
}
```