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

    std::cout << "hello wrold\n";
}