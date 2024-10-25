#include <string>

class X {
 public:
  X() : a_(0), b_(0.), c_("hello world") {}
  X(int a) : a_(a), b_(0.), c_("hello world") {}
  X(double b) : a_(0), b_(b), c_("hello world") {}
  X(const std::string& c) : a_(0), b_(0.), c_(c) {} 
 private:
  int a_;
  double b_;
  std::string c_;
};