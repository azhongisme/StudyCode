#include <unistd.h>
#include <iostream>
#include <errno.h>
#include <cstring>
#include <sys/stat.h>
#include <sys/types.h>

#if 1
int main() {
    // rename("b.cpp", "c.cpp");
    remove("c.cpp");
}
#endif

#if 0
int main(int argc, char* argv[]) {
    struct stat st;
    if (stat(argv[1], &st) != 0) {
        std::cout << "stat(" << argv[1] <<"):" << strerror(errno) << "\n";
        return -1;
    }
    if (S_ISREG(st.st_mode)) {
        std::cout << argv[1] << "是文件， mtime = " << st.st_mtime << ",size = " << st.st_size << "\n";
    }
    if (S_ISDIR(st.st_mode)) {
        std::cout << argv[1] << "是目录， mtime = " << st.st_mtime << ",size = " << st.st_size << "\n";
    }
}
#endif

#if 0
int main() {
    int result = access("b.cpp", F_OK);
    std::cout << result << "\n";
    std::cout << strerror(errno) << "\n";
}
#endif