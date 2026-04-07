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
#include <ctime>
//----------------------------------------------------------------------------------------------------------------------
///
/// explanation
///
/// @param param
///
/// @return return
//
#ifdef PART1

long int do_something(void) noexcept
{
  std::this_thread::sleep_for(std::chrono::seconds(5));
  std::cout << "Finishing everything...";
  return -1231231;
}

template <typename T>
auto calc_distance(const T& val1_x, const T& val2_x, const T& val1_y, const T& val2_y)
{
 auto dx = val2_x - val1_x;
 auto dy = val2_y - val1_y;
 return std::sqrt(dx * dx + dy * dy);
}
class Time 
{
  public:
    Time(unsigned short int hour, unsigned short int minute, unsigned short int second):
      hour_{hour}, minute_{minute}, second_{second}{}

    void set_day(const std::string& day){day_ = day;}
    void set_day(std::string&& day){day_ = std::move(day);}

    void set_month(std::string&& month){month_ = std::move(month);}
    void set_month(const std::string& month){month_ = month;}
    void print_data(void) const noexcept 
    {
      std::cout << "Current date: " << day_ << "." << month_ << ", time: " << hour_ << ":" << minute_ << ":" << second_ << 
        std::endl;
    }
  private:
    std::string day_;
    std::string month_;
    unsigned short int hour_, minute_, second_;
};
void calc_data(std::promise<Time>& promise)
{
  std::this_thread::sleep_for(std::chrono::seconds(3));//doing computation
  promise.set_value(Time(12,12,12));//triggers the .get_value() for the future to unbloc
}
double exception_func(double number)
{
  std::this_thread::sleep_for(std::chrono::seconds(2));
  std::cout << "Throwing an std::out_of_range exception. Value taken: " << number << std::flush;
  throw std::out_of_range("number is out of range");
  return number;
}
int main(void)
{
  using namespace std;
  // std::future<long int> fut = std::async(std::launch::async, do_something);
  // std::cout << std::endl << "the searched-for value is: " << std::endl << fut.get();
  std::promise<Time> prom;
  std::future<Time> fut = prom.get_future();//assign to future
  std::thread t(calc_data, std::ref(prom));

  //get the vlaue form future:
  std::future<double> res;
  fut.get().print_data();
 
  res = std::async(std::launch::async, exception_func, -234.234234234);
  try {
    std::cout << res.get() << " is the value of the future."; 

  } catch (const std::exception& e) {
    std::cout << std::endl << "Exception caught successfully.";

  }
  std::time_t time = std::chrono::system_clock().to_time_t(std::chrono::system_clock().now());
  std::cout << std::ctime(&time);

  t.join();//wait for the thread
  return 0;
}
#endif
#include <barrier>
std::barrier b(2, []{std::cout << "All threads have reached this point. Trying to recompute.";});

void do_something(void)
{
  std::this_thread::sleep_for(std::chrono::seconds(5));
  std::cout << "Finished a really long computation.";
  b.arrive_and_wait();
}

int main(void)
{
  std::future<void> f = std::async(std::launch::async, do_something);//launch an async thread
                                                                     //wait for 2 s, if finished continue, if not throw
  if(f.wait_for(std::chrono::seconds(2)) == std::future_status::ready)
    std::cout << "Result finished in time, continue...." << std::endl;
  else
    std::cout << "Sorry, couldn't finish in time, aborting..." << std::endl;
  std::thread t{do_something};
  std::cout << std::endl << "Number of threads supported by my hardware: " << std::thread::hardware_concurrency() << std::endl;

  t.join();
  return 0;
}
