#include <iostream>
#include <vector>
#include <cstdlib> // 包含 rand() 和 srand()
#include <ctime>   // 包含 time()

int i = 0;

class MyException : public std::exception {
public:
    const char* what() const noexcept override {
        return "MyException occoured";
    }
};

class DBConnection {
public:
    static DBConnection create();
    void close();
};

DBConnection DBConnection::create() {
    DBConnection db;
    return db;
}

void DBConnection::close() {
    std::srand(static_cast<unsigned int>(std::time(nullptr))); //为rand使用当前时间设置种子值
    int i = std::rand();
    std::cout << i << "\n";
    i %= 2;
    if (i == 0) {
        throw MyException();
    } else {
        std::cout << "close success\n";
    }
}

class DBConn {
public:
    ~DBConn() {
        // try { db.close(); }
        // catch (const MyException& e) {
        //     std::cerr << "caught a exception\n" << e.what() << "\n";
        //     std::abort();
        // }
        // std::cout << "continue\n";
        if (!closed) {
            try { db.close(); }
            catch ( const MyException& e ) {
                std::cerr << "caught a exception\n" << e.what() << "\n";
                std::abort();
            }
        }
    }
    void close () {
        db.close();
        closed = true;
    }
private:
    DBConnection db;
    bool closed;
};

int main() {
    DBConn dbconn;
    dbconn.close();
}