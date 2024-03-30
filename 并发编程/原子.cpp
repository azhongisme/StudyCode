#include <atomic>
#include <thread>
#include <iostream>

int main() {
    std::atomic_int n{1};
    n += 5;
    
}