#include <iostream>
#include <thread>
#include <future>
#include <mutex>
#include <condition_variable>

void task(int a, int b, std::promise<int>& ret) {
  ret.set_value(a + b);
}

int main() {
  int ret = 0;

  std::promise<int> p;
  std::future<int> f = p.get_future();

  std::thread work_thread(task, 1, 2, std::ref(p));

  //future调用get会阻塞，知道promise调用set_value才会停止阻塞
  //get只能进行一次
  std::cout << "return value is " << f.get() << std::endl;

  work_thread.join();
}

#if 0

std::condition_variable cv;

void task(int a, int b, int& ret) {
  ret = a + b;
  cv.notify_one();
}

int main() {
  int ret = 0;
  
  std::thread work_thread(task, 1, 2, std::ref(ret));

  std::mutex latch;

  std::unique_lock<std::mutex> lock(latch);
  
  cv.wait(lock);

  std::cout << "return value is " << ret << std::endl;

  work_thread.join();
}
#endif