#include <memory>
#include <iostream>
#include "cat.h"
#include "cat.cpp"



int main() {
    std::shared_ptr<Cat> p1 = std::make_shared<Cat>("p1");
    std::weak_ptr<Cat> w_p1(p1);
    std::cout<< "w_p1 count : " << w_p1.use_count() << '\n';
    std::shared_ptr<Cat> p2= w_p1.lock();
    std::cout<< "w_p1 count : " << w_p1.use_count() << '\n';

    std::shared_ptr<Cat> p3 = std::make_shared<Cat>("p3");
    std::shared_ptr<Cat> p4 = std::make_shared<Cat>("p4");

    p3->set_friend(p4);
    p4->set_friend(p3);
}