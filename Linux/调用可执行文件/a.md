# system()
把需要执行的程序和参数用一个字符串传给system()
```cpp
#include <stdlib.h>
int system(const char* string);
```
返回值：

1）如果执行程序失败，system()返回非0

2）如果执行程序成功，并且程序的终止状态是0,system()返回0

3）如果执行程序成功，并且程序的终止状态是非0,system()返回非0
```cpp
#include <iostream>
#include <stdlib.h>
#include <filesystem>

int main() {
    std::filesystem::path path = std::filesystem::current_path();
    std::string operateor_file = "ls /" + path.string() + "/";
    int res = system(operateor_file.c_str());
    std::cout << res << "\n";
    operateor_file = path.string() + "/b";
    system("/home/azhong/Desktop/cpp/StudyCode/Linux/调用可执行文件/b");
    system(operateor_file.c_str());
    char* str = "pwd";
    system(str);
}
```

# exce函数族
```
int execl(const char* path, const char* arg, ...);
int execlp(const char* file, const char* arg, ...);
int execle(const char* path, const char* arg, ..., char* const envp[]);
int execv(const char* path, char* const argv[]);
int execvp(const char* file, char* const argv[]);
int execvpe(const char* file, char* const argv[], char* const envp[]);
```
`execl`和`execv`
```cpp
#include <iostream>
#include <unistd.h>
#include <filesystem>
#include <string.h>
#include <errno.h>

int main() {
    std::filesystem::path path = std::filesystem::current_path();
    int res = execl("/bin/ls", "ls", path.string().c_str(), NULL);
    std::cout << strerror(errno) << "\n";
}
```
```cpp
#include <iostream>
#include <unistd.h>
#include <filesystem>
#include <string.h>
#include <errno.h>

int main() {
    std::filesystem::path path = std::filesystem::current_path();
    // int res = execl("/bin/ls", "ls", path.string().c_str(), NULL);
    // std::cout << strerror(errno) << "\n";

    char* args[10];
    args[0] = const_cast<char*>("/bin/ls");
    // args[1] = const_cast<char*>(path.native().c_str()); //native()避免转义字符
    args[1] = const_cast<char*>(".");
    args[2] = NULL;

    execv("/bin/ls", args);
}
```
新进程与原进程编号相同，但是新进程取代原进程的代码段、数据段和堆栈

exec函数之后的代码都不会执行