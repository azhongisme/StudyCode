#include <mutex>
#include <thread>
#include <iostream>
#include <vector>
#include <shared_mutex>

std::shared_mutex s_mutex;
std::string str = "原始内容";

void readloop() {
    std::string content;
    {
        std::shared_lock lock(s_mutex);
        content = str;
    }
    std::cout<<"str = "<<content<<'\n';
}

void writeloop(std::string s) {
    {
        std::lock_guard file_lock (s_mutex);
        str = s;
    }
    
}

int main() {
    std::thread t(writeloop,"no");
    std::thread t2(writeloop, "ok");
    std::thread t3(readloop);
    std::thread t4(readloop);
    t.join();
    t2.join();
    t3.join();
    t4.join();
}