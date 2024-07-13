# 目录操作函数
## 获取当前目录
```cpp
char* getcwd(char* buf, size_t size);
char* get_current_dir_name(void);
```
```cpp
#include <iostream>
#include <unistd.h>

int main() {
    char path1[256]; //Linux系统目录最大长度255
    getcwd(path1, 256);
    std::cout << path1 << "\n";

    char* path2 = get_current_dir_name();
    std::cout << path2 << "\n";
    delete path2;
}
```

## 切换工作目录
```cpp
int chdir(const char* path);
//0 成功， 其他失败
```

## 创建目录
```cpp
#include <sys/stat.h>
#include <iostream>

int main() {
    int result = mkdir("demo", 0755);
    std::cout << result << "\n";
    //0 成功， 其他失败
}
```

## 删除目录
```cpp
#include <unistd.h>
#include <iostream>

int main() {
    int result = rmdir("demo");
    std::cout << result << "\n";
}
```

# 获取目录中文件的列表
（1）opendir()打开目录

（2）readdir()读取目录

（3）closedir()关闭目录

## 目录指针DIR*
## dirent结构体
```cpp
struct dirent
  {
#ifndef __USE_FILE_OFFSET64
    __ino_t d_ino;
    __off_t d_off;
#else
    __ino64_t d_ino;
    __off64_t d_off;
#endif
    unsigned short int d_reclen;
    unsigned char d_type;   //文件类型，多种取值，其中8-常规文件， 4-目录 
    char d_name[256];		//文件名或目录名
  };
```
## 例子
```cpp
#include <dirent.h>
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        return -1;
    }
    DIR* dir;

    if ((dir = opendir(argv[1])) == 0) {
        return -1;
    }

    dirent* stdinfo = nullptr;

    while (1) {
        if ((stdinfo = readdir(dir)) == 0) {
            break;
        }

        std::cout << "文件名：" << stdinfo->d_name <<"， 文件类型：" << (int)stdinfo->d_type << "\n";
    }

    closedir(dir);
}
```