//复制对象时勿忘其每一个成分
//确保复制每一个local成员变量， 调用所有base classes内的适当的copying函数
#include <iostream>

typedef unsigned short month;
typedef unsigned short day;

class Date{
public:
  Date() = default;
  Date(const month& m, const day& d) : mm(m), dd(d) {}
private:
  month mm;
  day dd;

  friend std::ostream& operator << (std::ostream& os, const Date& date);
};

void logCall(const std::string& funcName) {
  std::cout << funcName << std::endl;
}

class Customer {
public:
  Customer() = default;
  Customer(const std::string name, const Date& date) : name(name), last_transaction(date) {}
  Customer(const std::string name, const month& m, const day& d) : name(name), last_transaction({m, d}) {}
  Customer(const Customer& rhs);
  Customer& operator = (const Customer& rhs);
  virtual void print();
private:
  std::string name;
  Date last_transaction;
};

class PriorityCustomer : public Customer {
public:
  PriorityCustomer(int p, const std::string name, const Date& date) : priority(p), Customer(name, date) {}
  PriorityCustomer(const PriorityCustomer& rhs);
  PriorityCustomer& operator = (const PriorityCustomer& rhs);
  void print() override;
private:
  int priority;
};

std::ostream& operator << (std::ostream& os, const Date& date) {
  os << date.dd << " " << date.mm;
  return os;
}

Customer::Customer(const Customer& rhs) : name(rhs.name), last_transaction(rhs.last_transaction) {
  logCall("Customer copy constructor");
}

Customer& Customer::operator = (const Customer& rhs) {
  logCall("Customer copy assignment operator");
  name = rhs.name;
  last_transaction = rhs.last_transaction;
  return *this;
}

void Customer::print() {
  std::cout << name << std::endl;
  std::cout << last_transaction << std::endl;
}

PriorityCustomer::PriorityCustomer(const PriorityCustomer& rhs) : priority(rhs.priority), Customer(rhs) {
  logCall("PriorityCustomer copy constructor");
}

PriorityCustomer& PriorityCustomer::operator = (const PriorityCustomer& rhs) {
  logCall("PriorityCustomer copy assignment operator");
  priority = rhs.priority;
  Customer::operator = (rhs);
  return *this;
}

void PriorityCustomer::print() {
  Customer::print();
  std::cout << priority << std::endl;
}

int main() {
  PriorityCustomer c1(1, "aaa", {12, 2});
  c1.print();
  PriorityCustomer c2(c1);
  c2.print();
  PriorityCustomer c3(2, "bbb", {12, 3});
  c3 = c1;
  c3.print();
}