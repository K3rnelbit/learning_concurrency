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
//wait for 1 second before checking if file with specific name contains a specific phrase, if it does, print a message and exit
//it should be implmemented on a separate, joined() thread:
void check_file(void);
bool done{false};
std::condition_variable cv;
std::mutex m;
int main(void)
{
  check_file();
  return 0;
}
void t_check_file(void)
{
  while(true)
  {
    std::ifstream i("file.txt");
    std::string phrase = "Done";
    if(i.is_open())
    {
      std::string line;
      std::getline(i, line);
      if(line == phrase)
      {
        {
          std::lock_guard<std::mutex> l{m};
          done = true;
        }
        cv.notify_one();
        return;
      }                                                                   
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));//sleep for 2 second
  }
}
void monitor_done(void)
{
  std::unique_lock<std::mutex>l{m};
  cv.wait(l, [&]{return done;});
  std::cout << "File contains the passphrase, exiting...";
}
void check_file(void)
{
  std::thread t{t_check_file};
  t.join();
  //another thread monitors the donevar:
  std::thread t2{monitor_done};
  t2.join();
}
