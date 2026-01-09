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
#include <thread>
//----------------------------------------------------------------------------------------------------------------------
///
/// explanation
///
/// @param param
///
/// @return return
//
//condition_variable:
#ifdef ONE
std::mutex m;
class E{public: int a;};
std::queue<E> small_queue;
std::condition_variable cv;

bool more_data_to_prepare(void) noexcept
{
  static int counter{0};
  if(counter++ < 10)
    return true;
  return false;
}
E prepare_data(void) noexcept
{
  return E{1};
}
void data_preparation_thread(void)
{
  while(more_data_to_prepare())
  {
    const E e = prepare_data();
    std::lock_guard<std::mutex> lock(m);//after this only a single thread can access the rest 
    small_queue.push(e);//single thread pushes e to small_queue
    cv.notify_one();
  }
}
void process(E e)
{
  std::cout << e.a;
}
bool is_last_chunk(E e)
{
  std::cout << e.a;
  if(!small_queue.size() || small_queue.size() == 1)
    return true;
  return false;
}
void data_processing_thread(void) 
{
  while(true)
  {
    std::unique_lock<std::mutex> lock{m};
    cv.wait(lock, []{return !small_queue.empty();});
    E data{small_queue.front()};
    small_queue.pop();
    lock.unlock();//unlock the mutex, it is no longer needed
    process(data);
    if(is_last_chunk(data))
      break;
  }
}
#endif
#ifdef PART2
template<typename T>
class thread_safe_queue
{
  private:
    std::queue<T> queue_;
    std::condition_variable cv_;
    mutable std::mutex m_;
  public:
    thread_safe_queue& operator=(const thread_safe_queue&) = delete;
    void push(T value)
    {
      std::lock_guard<std::mutex> lock{m_};
      queue_.push(value);
      cv_.notify_one();
    }
    void wait_and_pop(T& value)
    {
      std::unique_lock<std::mutex> lock{m_};
      cv_.wait(lock, [this]{return queue_.size();});//pass in this to make all vars in the object accessible inside the lambda func
      value = queue_.front();
      queue_.pop();
    }
    friend bool operator==(const thread_safe_queue<T>& q1, const thread_safe_queue<T>& q2)
    {
      std::unique_lock<std::mutex> l1{q1.m_, std::defer_lock};
      std::unique_lock<std::mutex> l2{q2.m_, std::defer_lock};
      std::lock(l1, l2);
      return q1.queue_ == q2.queue_;
    }
};

class CustomType{
  private:
    int val{0};
  public:
    explicit CustomType(int v = 0):val{v}{}

    friend bool operator==(const CustomType& ct1, const CustomType& ct2) noexcept
    {
      return (ct1.val == ct2.val);
    }
};
bool more_data_to_prepare(void)
{
  static int counter{0};
  while(counter++ < 10)
    return true;
  return false;
}
CustomType fetch_data(void)
{
  return CustomType{};
}
thread_safe_queue<CustomType> q;
void t_prepare()
{
  while(more_data_to_prepare())
  {
    const CustomType data = fetch_data();
    q.push(data);
  }
}
bool process(CustomType d)
{
  if(d == CustomType{})
    return true;
  return false;
}
bool is_last_chunk(const CustomType& d)
{
  if(d == CustomType{})
    return true;
  return false;
}
void t_process(void)
{
  while(true)
  {
    CustomType d;
    q.wait_and_pop(d);
    process(d);
    if(is_last_chunk(d))
      return;
  }
}
std::mutex mutex_;
std::condition_variable cv;
unsigned short int processed{0};
unsigned long example(void)
{
  {
    std::lock_guard<std::mutex>l{mutex_};
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));//simulate work
    std::cout << "T_" << processed + 1 << "'s id: " << std::this_thread::get_id() << std::endl;
    ++processed; 
  }
  cv.notify_one();//
  return 34;//return result;
}

int main(void)
{
  using namespace std;
  cout << "Hello bood";
  std::thread t1{example};
  std::thread t2{example};
  {
    std:: unique_lock<std::mutex>l{mutex_};
    cv.wait(l, []{return processed == 2;});//wait data is processed
  }

  t1.join();
  t2.join();
  std::cout << "data for thread" << std::this_thread::get_id() << "has been processed" << std::endl;
  std::cout << "Main thread's id: " << std::this_thread::get_id() << std::endl;

  return 0;
}
#endif

void do_something(std::promise<const char *>&& promise) noexcept
{
  std::this_thread::sleep_for(std::chrono::seconds(2));
  promise.set_value("An unusually long interruption has not occured.");
}
int main(void)
{
  std::promise<const char *> p;
  std::future<const char *>f{p.get_future()};
  
  std::thread t{do_something, std::move(p)};
  std::cout << f.get();
  t.join();

  return 0;
}
