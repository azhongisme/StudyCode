#include <errno.h>
#include <iostream>
#include <cstring>
#include <sys/stat.h>


int main() {
    int iret = mkdir("/tmp/aaa/bbb/ccc", 0755);
    perror("调用mkdir(/tmp/aaa/bbb/ccc)失败");
}

#if 0
int main() {
    int iret = mkdir("/tmp/aaa", 0755);

    std::cout << "iret = " << iret << "\n";
    std::cout << errno << ":" << strerror(errno) << "\n";
}
#endif

#if 0
int main() {
    int ii;
    for (ii = 0; ii < 150; ++ii) {
        std::cout << ii << " : " << strerror(ii) << "\n";
    }
}
# endif