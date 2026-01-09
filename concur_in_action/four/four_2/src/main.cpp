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
#include <unistd.h>
//----------------------------------------------------------------------------------------------------------------------
///
/// explanation
///
/// @param param
///
/// @return return
//

int do_something(void) noexcept
{
  std::this_thread::sleep_for(std::chrono::seconds(2));
  return 44;
}
void one(void) noexcept
{
  using namespace std;
  std::future<int> r = std::async(std::launch::async, do_something);
  std::cout << "Starting to wait for the result..." << std::endl;

  int result = r.get();
  std::cout << "The result: " << result << std::endl;

  //example two:
  std::shared_future<int> r2{std::async(std::launch::async, do_something)};//launch new thread, start do_something in that thread
  
  std::thread t1{[&r2]{std::cout << "Getting something out: " << r2.get() << std::endl;}};
  std::thread t2{[&r2]{std::cout << "Getting something else out: " << r2.get() << std::endl;}};
  //what happens in the background::
  //the reading thread reads the line 36 with r2, sees std::luanch::async, launches another thread, asynchronously, that runs do_something
  //then another thread t1 is created, this new thread runs the starts and runs the lambda func, in the lambda the thread t1 reads
  //the r2.get() and seeks for the result/exception, since none are provided yet, it blocks(puts itself in a waiting state)
  //the thread in main meanlwhile continues to t2, where t2 again tries to access the r2.get(), fails and blocks
  //now both t1,t2 are waiting for r2.get() to finish, main thread is waiting for t1,t2
  //when r2.get() finishes, the thread t1 unblocks, retrieves the result and finishes its func, destroying itself at the end
  //same happens for the thread t2
  t1.join();
  t2.join();//main should wait for both threads to finish

}
struct X 
{
  void f(int,const std::string&);
  std::string g(const std::string&);

};
X x;
void two(void)
{
  std::future<void> f{std::async(std::launch::async, &X::f, &x, 44, "something valuable")};
  std::future<std::string> f2{std::async(std::launch::async, &X::g, &x, "wow")};

}
int main(void)
{
  return 0;
}
