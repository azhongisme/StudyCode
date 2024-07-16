#include <iostream>

class Uncopyable {
public:
    Uncopyable() {}
    ~Uncopyable() {}
private:
    Uncopyable(const Uncopyable&);
    Uncopyable& operator = (const Uncopyable);
};

class HomeForSale : private Uncopyable {

};

int main() {

}