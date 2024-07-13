#include <iostream>

# if 0
template <typename T>
class NamedObject {
public:
    NamedObject(const char* name, const T& value);
    NamedObject(const std::string& name, const T& value);
    void show() const;
private:
    std::string nameValue_;
    T objectValue_;
};

template  <typename T>
NamedObject<T>::NamedObject(const char* name, const T& value) : nameValue_(name), objectValue_(value) {}

template  <typename T>
void NamedObject<T>::show() const{
    std::cout << "name is " << nameValue_ << "\n";
    std::cout << "value is " << objectValue_ << "\n";
}

int main() {
    NamedObject<int> no1("Smallest Prime Numeber", 2);
    NamedObject<int> no2(no1);
    no2.show();
}
# endif

#if 1
template<typename T>
class NameObject {
public:
    NameObject(std::string& name,const T& value) : name_(name), value_(value) {}
    void show() {
        std::cout << "name is " << name_ << "\n";
        std::cout << "value is " << value_ << "\n";
    }
private:
    std::string& name_;
    const T value_;
};

int main() {
    std::string newDog = "aaa";
    std::string oldDog = "bbb";
    NameObject<int> p(newDog, 1);
    NameObject<int> s(oldDog, 3);
    p = s;
    p.show();
}
#endif