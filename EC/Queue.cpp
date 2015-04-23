#include "Queue.h"

  string Queue::pop()
  {
    unique_lock<mutex> mlock(mutex_);
    while (queue_.empty())
    {
      cond_.wait(mlock);
    }

    string cnt = queue_.front();
    queue_.pop_front();
    return cnt;
  }

  void Queue::pop(string& item)
  {
    unique_lock<mutex> mlock(mutex_);
    while (queue_.empty())
    {
      cond_.wait(mlock);
    }
    item = queue_.front();
    queue_.pop_front();
  }

  void Queue::push(const string& item)
  {
    
    unique_lock<mutex> mlock(mutex_);
    queue_.push_back(item);
    mlock.unlock();
    cond_.notify_one();
  }



