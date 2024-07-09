// Make sure that objects are initialied before they're used
#include <iostream>
#include <list>

class PhoneNumber {
    std::string number;
};

class ABEntry {
public:
    ABEntry(const std::string& name, const std::string& address,
    const std::list<PhoneNumber>& phones);
private:
    std::string theName;
    std::string theAddress;
    std::list<PhoneNumber> thePhones;
    int numTimesConsulted;
};

ABEntry::ABEntry(const std::string& name, const std::string& address,
        const std::list<PhoneNumber>& phones) {
    //这些都是赋值而非初始化
    theName = name;
    theAddress = address;
    thePhones = phones;
    numTimesConsulted = 0;
}

ABEntry::ABEntry(const std::string& name, const std::string& address,
    const std::list<PhoneNumber>& phones) 
    : theName(name), theAddress(address), thePhones(phones), numTimesConsulted(0) {}
    //现在是初始化

class FileSystem {
public:
    std::size_t numDisks() const;//众多成员函数之一 
};

extern FileSystem ths;

class Directory {
public:
    Directory()
}

int main() {
    std::string name = "azhong";
    std::string address = "address";
    std::list<PhoneNumber> phones;
    ABEntry entry(name, address, phones);
}