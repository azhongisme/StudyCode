#include <iostream>
#include <unistd.h>

#if 0
int main() {
    char path1[256]; //Linux系统目录最大长度255
    getcwd(path1, 256);
    std::cout << path1 << "\n";

    char* path2 = get_current_dir_name();
    std::cout << path2 << "\n";
    delete path2;
}
#endif

#if 0

#include <sys/stat.h>
#include <iostream>

int main() {
    int result = mkdir("demo", 0755);
    std::cout << result << "\n";
}

#endif

# if 0

#include <unistd.h>
#include <iostream>

int main() {
    int result = rmdir("demo");
    std::cout << result << "\n";
}

#endif

#if 1

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

#endif