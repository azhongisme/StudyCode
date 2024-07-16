#include <iostream>

class TimeKeeper {
public:
    TimeKeeper() {}
    ~TimeKeeper() {}
};

TimeKeeper* getTimeKeeper() {
    TimeKeeper* ptr = new TimeKeeper();
    return ptr;
}

class AtomicClock : public TimeKeeper {};
class WaterClock : public TimeKeeper {};
class WristWatch : public TimeKeeper {};

class AWOV {
public:
    virtual ~AWOV() = 0;
};

AWOV::~AWOV() {} //提供一份定义

int main() {
    TimeKeeper* ptr = getTimeKeeper();
    delete ptr;
}