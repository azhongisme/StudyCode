#include <iostream>
#include <string>

template<class T>
void show_type(T t) {
  std::cout << typeid(t).name() << std::endl;
}

template<class T>
void normal_forwarding(T&& t) {
  show_type(static_cast<T&&>(t));
}

std::string get_string() {
  return "hello world";
}

int main() {
  std::string s = "hello world";
  normal_forwarding(s);
  normal_forwarding(get_string());
}