#include <iostream>

void segmentFault() {
    // char* ch = new char(10);
    char* ch = nullptr;
    *ch = 3;
}

int main() {
    segmentFault();
}