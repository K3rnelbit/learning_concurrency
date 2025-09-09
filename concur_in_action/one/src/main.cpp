//---------------------------------------------------------------------------------------------------------------------
// c++.cc
//
// <Explanation of the program ...>
// <... May have multiple lines.>
//
//---------------------------------------------------------------------------------------------------------------------
//
#include "main.h"
#include <stdexcept>
#ifdef PART_1
#include <filesystem>
#include <functional>
#include <numeric>

//---------------------------------------------------------------------------------------------------------------------
///
/// This is an example header comment. Here, you explain, what the
/// function does. Copypaste and adapt it!
///
/// @param numbers an array to check
/// @param length The length of the array. If this text is too long you can
///
/// @return 
//

void hello(void)
{
  std::cout << "Hello World";
}
class Example
{
  public:
    void operator()(char *text)

    {
      std::cout << text << std::endl;
      std::cout << "Finished, deleting the alloced memory.";
      delete[] text;
    }
};
//class that lock_guards thread:
class lock_thread
{
  public:
    explicit lock_thread(std::thread thread):t_{std::move(thread)}{}
    ~lock_thread()
    {
      if(t_.joinable())
        t_.join();//else exit, since the thread is already handled outside of the class scope
    }
    lock_thread(const lock_thread&)=delete;
    lock_thread& operator=(const lock_thread &) = delete;

    lock_thread(lock_thread&&) = default;
    lock_thread& operator=(lock_thread&&) = default;

  private:
    std::thread t_;

};
static void modify_int(int& integer)
{
  integer +=55;
}
void f(void)
{
  std::cout << "\"f\" has been called";
}
void g(void)
{
  std::cout << "\"g\" has been called";
}

template<typename It, typename T>
void accumulate_block(const It& block_start, const It& block_end, T& result)
{
  result = std::accumulate(block_start, block_end, result);
}

template<typename It, typename T>
auto parallel_accumulate(It first, It last, T init) -> T
{
  const unsigned long int distance{std::distance(first, last)};
  if(!distance)
    return init;//if distance is 0 = this is the same element, return

  const unsigned long int min_per_thread{25};
  const unsigned long int max_threads{(distance + min_per_thread - 1) / min_per_thread};
  //distance / min_per_thread floors, we need to top the distance, if we add full min_per_thread, and distance is multiple
  //of min_per_thread, we will get one extra thread, so to account for that -1
  const unsigned long int hardware_threads{std::thread::hardware_concurrency()};

  unsigned long int number_of_threads{std::min(hardware_threads!=0?hardware_threads:2,max_threads)};
  const unsigned int block_size{distance / number_of_threads};

  std::vector<T>results(number_of_threads);
  std::vector<std::thread>threads(number_of_threads - 1);//-1 because one is already running (in main)
  It block_start{first};
  for(unsigned long int i = 0; i < (number_of_threads - 1);++i)
  { //move through each thread except for the thread starting main:
    It block_end = block_start;
    std::advance(block_end, block_size);//update block_end
    threads[i] = std::thread(accumulate_block, block_start, block_end,std::ref( results[i]));
    std::cout << "\nCurrent thread's id: " << threads[i].get_id();
    //here results[i] is treated as copy, so we have to cast it to std::ref()
    block_start = block_end;

  }
  //join the threads:
  std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));

  //for the very last thread:
  accumulate_block(block_start, block_start + block_size, results[number_of_threads - 1]);
  //compute total:
  unsigned long long int total{0};
  for(auto e: results)
    total += e;
  return total;
}
int main(void)
{
  using namespace std;
  // std::thread new_thread{
  // []()
  // {
  //  hello(); 
  // // }};
  // // new_thread.join();
  // std::cout << "Thread finishe.d" << std::endl;
  
  // char *text = (char*)(malloc(sizeof(char) * 25));
  // char str[] = "An ordinary string.";
  // unsigned short int counter =0;
  // for(; str[counter] != '\0'; ++counter)
    // text[counter] = str[counter];
  // text[counter] = '\0';

  // Example e;
  // std::thread t(e, text);
  // t.join();
  char *text = new char[40]{"Anything but unusual"};
  Example e;
  std::thread t(e,text);
  t.join();
  Example e1;
  lock_thread locked_thread(std::thread(e1,new char[40]{"Another one:"}));
  std::cout << "The thread exist(joins) safely";

  
  int int_to_modify{-234};
  {

    lock_thread new_lock(std::thread{modify_int, std::ref(int_to_modify)});
  }
  std::cout << "\nInt has value: " << int_to_modify << std::endl;

  std::thread t1{f};
  std::thread t2{std::move(t1)};//move resources from t1 to t2
  t1 = std::thread(f);//move-assing a new value to t1(t1 is in valid but unspecified state)
  
  std::thread t3{std::move(t2)};
  t1.join();
  t3.join();
  std::cout <<"can run concurrently: " << std::thread::hardware_concurrency() << std::endl;
  std::cout <<"THread one's id:" << t1.get_id();
  std::cout << std::endl << ",two: " << t2.get_id();
  
  return 0;
}
#endif
#include <fstream>

int main(void)
{
  std::thread::id main_thread_id = std::this_thread::get_id();
  std::cout << "The id of the main thread: " << main_thread_id;
  std::cout << system("pwd");
  std::fstream stream;
  stream.open("thread_id.txt", std::ios::app);
  if(!stream.is_open())
    throw std::runtime_error("could not open the stream");
  stream << "Thread_id: " << main_thread_id << "\n";

  return 0;
}
