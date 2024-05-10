//尽量以const.enum,inline替换#define
#include <iostream>

class GamePlayer {
public:
    void GetAddress() {
        std::cout<< &NumTurns;
    }
private:
    static const int NumTurns = 5;
    int scores[NumTurns];
};

void f(const int& x) {
    std::cout<< x << std::endl;
}

#define CALL_WITH_MAX(a, b) f((a) > (b) ? (a) : (b))


const int GamePlayer::NumTurns;

template<typename T>
inline void callWithMax(const T& a, const T& b) {
    f(a > b ? a : b);
}

int main() {
    int a = 5, b = 0;
    // CALL_WITH_MAX(++a, b);
    // CALL_WITH_MAX(++a, b + 10);
    callWithMax(++a, b);
    callWithMax(++a, b + 10);
    std::cout<< a;
}