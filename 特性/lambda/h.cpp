template<class T>
void g(T) {}

struct Foo {
  int n = 0;
  void f(int a) {
    g([=](int k) { return n + a * k; });
  }
};

int main() {
  
}