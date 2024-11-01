#include <iostream>

int main() {
  int x = 999;
  const int y = 999;
  const int z = 99;
  const double cdb = 99.9;
  char c1 = x;
  char c2{x};//warning
  // char c3{y};//error
  char c4{z};
  unsigned char uc1{5};
  // unsigned char uc2{-1};//error
}