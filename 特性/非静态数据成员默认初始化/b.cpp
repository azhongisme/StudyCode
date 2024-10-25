#include <iostream>
struct S {
  int y : 8 = 11;  //y占用8位
  int z : 4 { 7 }; //z占用4位
};

int a;
struct S2 {
  int y : true ? 8 : a = 42;
  int z : 1 || new int { 0 };
};

struct S3 {
  int y : (true ? 8 : a) = 42;
  int z : (1 || new int) {0};
};

int main() {
  S s;
  std::cout << s.y << " " << s.z << std::endl;
  S2 s2;
  std::cout << s2.y << " " << s2.z << std::endl;
  S3 s3;
  std::cout << s3.y << " " << s3.z << std::endl;
}