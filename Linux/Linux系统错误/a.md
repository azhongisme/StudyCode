# strerror
```cpp
char* strerror(int errnum); //非线程安全
int strerror_r(int errnum, char* buf, size_t buflen); //线程安全
```
示例
```cpp
#include <iostream>
#include <cstring>

int main() {
    int ii;
    for (ii = 0; ii < 150; ++ii) {
        std::cout << ii << " : " << strerror(ii) << "\n"; // 一共133个错误代码
    }
}
```
```cpp
#include <errno.h> //errno全局变量头文件
#include <iostream>
#include <cstring>
#include <sys/stat.h>

int main() {
    int iret = mkdir("/tmp/aaa", 0755);

    std::cout << "iret = " << iret << "\n";
    std::cout << errno << ":" << strerror(errno) << "\n";
}
```
# perror
在<stdio.h>中声明，用于在控制台显示最近一次系统错误额的详细信息
```cpp
void perror(const char* s);
```
```cpp
int main() {
    int iret = mkdir("/tmp/aaa/bbb/ccc", 0755);
    perror("调用mkdir(/tmp/aaa/bbb/ccc)失败");
}
```

# 注意事项
并不是所有的库函数调用失败时都会设置errno的值，属于系统调用的函数才会设置errno

errno不会在函数调用成功的时候置0