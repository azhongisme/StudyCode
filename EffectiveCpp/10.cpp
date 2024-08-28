//令operator返回一个reference to *this
#include <iostream>

class Widget{
public:
    Widget& operator = (const Widget& rhs) {
        return *this;
    }
    Widget& operator += (const Widget& rhs) {
        this->num_ += rhs.num_;
        return *this;
    }
private:
    int num_;
};
