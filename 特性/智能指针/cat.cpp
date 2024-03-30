#include "cat.h"

Cat::Cat(std::string name) : name_(name) {
    std::cout<<"构造 "<<name<<'\n';
}
Cat::~Cat() {
    std::cout<<"析构 "<<name_<<'\n';
}