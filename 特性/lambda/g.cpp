int main() {
  auto foo = [](auto a) { return a; };
  int three = foo(3);
  char const* hello = foo("hello");
}