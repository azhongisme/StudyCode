#include <iostream>
#include <cstring>

class TextBlock {
public:
    TextBlock(const std::string str) : text(str) {}
    const char& operator[] (std::size_t position) const
    { return text[position]; }
    //operator[] for const 对象
    char& operator[] (std::size_t position)
    { return const_cast<char&>(static_cast<const TextBlock&>(*this)[position]); }
    //operator[] for non-const 对象

private:
    std::string text;
};

class CTextBlock {
public:
    std::size_t length() const;
private:
    char* pText;
    mutable std::size_t textLength;
    mutable bool lengthIsvalid;
};

std::size_t CTextBlock::length() const {
    if (!lengthIsvalid) {
        textLength = std::strlen(pText);
        lengthIsvalid = true;
    }
    return textLength;
}

int main() {
    TextBlock tb("hello");
    tb[0] = 'x';
    std::cout<<tb[0];
    const TextBlock ctb("hello");
    // ctb[0] = 'x'; //error
    std::cout<<ctb[0];
}