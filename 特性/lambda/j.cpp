#include <map>
#include <string>

auto greater = [](auto x, auto y) { return x > y; };

int main() {
  std::map<std::string, int, decltype(greater)> mymap1, mymap2; 
  mymap1 = mymap2;
}