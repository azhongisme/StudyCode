//在operator=中处理自我赋值
#include <iostream>
class Bitmap{

};

// class Widget{
// public:
//     Widget& operator = (const Widget& rhs) {
//         if (this == &rhs) return *this;
        
//         delete pb;
//         pb = new Bitmap(*rhs.pb);
//         return *this;
//     }
// private:
//     Bitmap* pb;
// };

class Widget{
public:
    Widget& operator = (const Widget& rhs) {
        //在复制pb所指东西之前别删除pb
        Bitmap* pOrig = pb;
        pb = new Bitmap(*rhs.pb);
        delete pOrig;
        return *this;
    }
private:
    Bitmap* pb;
};

class Widget{
public:
    Widget& operator = (const Widget& rhs) {
        Widget temp(rhs);
        swap(temp);
        return* this;
    }
private:
    void swap(Widget& rhs) {
        Bitmap* pOrig = pb;
        pb = new Bitmap(*rhs.pb);
        rhs.pb = pOrig;
    }
    Bitmap* pb;
};