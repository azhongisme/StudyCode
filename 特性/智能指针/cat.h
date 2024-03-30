#ifndef CAT_H
#define CAT_H

#include <string>
#include <iostream>
#include <memory>

class Cat {
public:
    Cat(std::string name = "mao");
    // Cat() = delete;
    ~Cat();
    void cat_info() const {
        std::cout<<name_<<'\n';
    }
    std::string get_name() const {
        return name_;
    }
    void set_name(const std::string& name) {
        this->name_ = name;
    }
    void set_friend(std::shared_ptr<Cat>c) {
        cat_friend_ = c;
    }
private:
    std::string name_;
    std::weak_ptr<Cat> cat_friend_;
};

#endif