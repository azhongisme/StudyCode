#include <iostream>

#if 0
class Transaction {
public:
    Transaction();
    virtual void logTransaction() const = 0;
};

Transaction::Transaction() {
    logTransaction();
}

void Transaction::logTransaction() const {
    std::cout << "logged\n";
}

class BuyTransaction : public Transaction {
public:
    virtual void logTransaction() const;
};

class SellTransaction : public Transaction {
public:
    virtual void logTransaction() const;
};

void BuyTransaction::logTransaction() const {
    std::cout << "buy is logged\n";
}

int main() {
    BuyTransaction b;
}
#endif

#if 1

class Transaction {
public:
    explicit Transaction(const std::string& logInfo);
    void logTransaction(const std::string& logInfo);
};

Transaction::Transaction(const std::string& logInfo) {
    logTransaction(logInfo);
}

void Transaction::logTransaction(const std::string& logInfo) {
    std::cout << logInfo << "\n";
}

class BuyTransaction : public Transaction {
public:
    BuyTransaction(std::string& logInfo) : Transaction(createLogString(logInfo)) {}
private:
    static std::string createLogString(std::string& logInfo);
};

std::string BuyTransaction::createLogString(std::string& logInfo) {
    return logInfo;
}

int main() {
    std::string str = "abc";
    BuyTransaction b(str);
}
#endif
