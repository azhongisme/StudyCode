#include <iostream>
#include <string>

class C {
 public:
  C(std::initializer_list<std::string>a) {
    for (const std::string *item = a.begin(); item != a.end(); ++item) {
      std::cout << item << " ";
    }
    std::cout << std::endl;
  }
};

int main() {
  C c{"hello" , "word"};
}