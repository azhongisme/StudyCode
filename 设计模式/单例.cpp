#include <iostream>


class Singleton {
public:
    static Singleton& get_instance();
    void show();
private:
    Singleton() {}
    Singleton(const Singleton&)  = delete;
    Singleton(const Singleton&&) = delete;
    Singleton& operator = (const Singleton&) = delete;
};

Singleton& Singleton::get_instance() {
    static Singleton s;
    return s;
}

void Singleton::show() {
    std::cout<< "This is Singleton" << std::endl;
}

int main() {
    Singleton& s = Singleton::get_instance();
    s.show();
}