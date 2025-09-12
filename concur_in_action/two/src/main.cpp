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
#include <functional>
#include <stack>
//----------------------------------------------------------------------------------------------------------------------
///
/// explanation
///
/// @param param
///
/// @return return
std::mutex m;
unsigned int counter{0};
void f(void) noexcept 
{
  std::lock_guard<std::mutex> g{m};
  ++counter;
}
void f2(void) noexcept
{
  std::lock_guard<std::mutex> g{m};//lock the mutex
  ++counter;
}
class some_data
{
  public:
    void do_sm(void) noexcept;
  private:
    int var;
    std::string str;
};



class wrapper
{
  public:
    template<typename Func>
      void process_data(Func func)
      {
        std::lock_guard<std::mutex> g{m_};//lcok the mutex
        func(some_data_);
      }
  private:
    std::mutex m_;
    some_data some_data_;
};
template<typename T, typename Container = std::deque<T>>
class stack 
{
  public:
    explicit stack(const Container& c);
    explicit stack(Container&& = Container());

}
int main(void)
{
  using namespace std;
  
  

  cout << "Hello World";
  return 0;
}
