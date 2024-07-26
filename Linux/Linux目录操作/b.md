# access()
用于判断当前用户对目录或文件的存取权限
```cpp
int access(const char* pathname, int mode);
#define R_OK 4 //读权限
#define W_OK 2 //写权限
#define X_OK 1 //执行权限
#define F_OK 0 //是否存在
```
当pathname满足mode权限返回0,不满足返回-1,errno被设置。

在实际开发中，access()主要用于判断目录或文件是否存在。
```cpp
#include <unistd.h>
#include <iostream>

int main() {
    int result = access("b.cpp", F_OK);
    std::cout << result << "\n";
}
```

# stat()
头文件#include<sys/stat.h>

用于获取path参数指定目录或文件的详细信息，保存到buf结构体中
```cpp
int stat(const char* path, struct stat* buf);
```

# utime
用于修改目录或文件的时间
```cpp
int utime(const char* filename, const struct utimbuf* times);
struct utimbuf {
    time_t actime;
    time_t modtime;
};
```

# rename
用于重命名目录或文件，相当与mv命令
```cpp
int rename(const char* oldpath, const char* newpath);

int main() {
    rename("b.cpp", "c.cpp");
}
```

# remove
用于删除目录或文件，相当与rm命令
```cpp
int remove(const char* pathname);
```