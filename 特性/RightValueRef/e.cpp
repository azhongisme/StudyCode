#include <iostream>

int main() {
  int i = 0;
  int &&k = static_cast<int&&>(i);
}