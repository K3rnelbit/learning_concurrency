//----------------------------------------------------------------------------------------------------------------------
// File: File name
//
// Date: YYYY-MM-DD
//
// Description: purpose
//
// Author: me
//----------------------------------------------------------------------------------------------------------------------
//
#include "main.h"
//----------------------------------------------------------------------------------------------------------------------
///
/// explanation
///
/// @param param
///
/// @return return
//
// std::mutex m1,m2;
// void f1(void)
// {
//   std::lock_guard<std::mutex> g1{m1};
//   //simulate work:
//   std::this_thread::sleep_for(std::chrono::milliseconds(100));
//   std::lock_guard<std::mutex> g2{m2};//lock the second mutex
//   std::cout << "Thread one finished.";
// }
// void f2(void)
// {
//   std::lock_guard<std::mutex> g1{m2};
//   std::this_thread::sleep_for(std::chrono::milliseconds(100));
//   std::lock_guard<std::mutex> g2{m1};
//   std::cout << "thread 2 finished.";
// }
thread_local unsigned long long int val{123};
std::recursive_mutex m;
void f()
{
  std::lock_guard<std::recursive_mutex>l{m};
  for(std::size_t i = 0; i < 5; ++i)
  {
    std::cout << "Thread: " << std::this_thread::get_id() << ", val:" << val++ << std::endl;
  }
}
std::mutex a1,a2,a3,a4;
int main(void)
{
  using namespace std;
  // std::thread t1{f1};
  // std::thread t2{f2};
  // t1.join();
  // t2.join();
  std::scoped_lock l(a1,a2,a3,a4);
  //4 mutexes are locked
  std::thread one,two;
  one = std::thread{f};
  two = std::thread{f};
  one.join();
  two.join();

  return 0;
}
