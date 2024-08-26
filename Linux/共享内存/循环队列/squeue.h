#include <cstring>
#include <iostream>

template <class T, int MaxLength>
class squeue {
private:
    bool inited_ {false};
    T data_[MaxLength];
    int head_;
    int tail_;
    int length_;
    squeue(const squeue &) = delete;
    squeue & operator = (const squeue &) = delete;
public:
    squeue() { init(); }
    //如果是共享内存，不会调用调用构造函数
    void init() {
        if (inited_ != true) {
            head_ = 0;
            tail_ = MaxLength - 1;
            length_ = 0;
            memset(data_, 0, sizeof data_);
            inited_ = true;
        }
    }
    
    bool push(const T &e) {
        if (full() == true) {
            std::cout<< "squeue is full, push is failed\n";
            return false;
        }

        tail_ = (tail_ + 1) % MaxLength;
        data_[tail_] = e;
        length_++;
        return true;
    }

    size_t size() {
        return length_;
    }

    bool full() {
        return length_ == MaxLength;
    }

    bool empty() {
        return length_ == 0;
    }

    T& front() {
        return data_[head_];
    }

    bool pop() {
        if (empty() == true) {
            std::cout << "squeue is empty, pop is failed\n";
            return false;
        }
        head_ = (head_ + 1) % MaxLength;
        length_--;
        return true;
    }

    void printQueue() {
        for (size_t i = head_; i != tail_; i = (i + 1) % MaxLength) {
            std::cout << data_[i] << " ";
        }
        std::cout << data_[tail_] << "\n";
    }
};