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