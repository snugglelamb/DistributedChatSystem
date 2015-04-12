//
// Copyright (c) 2013 Juan Palacios juan.palacios.puyana@gmail.com
// Subject to the BSD 2-Clause License
// - see < http://opensource.org/licenses/BSD-2-Clause>
//

#ifndef CONCURRENT_QUEUE_
#define CONCURRENT_QUEUE_

#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <list>
#include <iostream>
template <typename T>
class Queue
{
 public:

  T pop()
  {
    std::unique_lock<std::mutex> mlock(mutex_);
    while (queue_.empty())
    {
      cond_.wait(mlock);
    }
    auto val = queue_.front();
    queue_.pop_front();
    return val;
  }

  void pop(T& item)
  {
    std::unique_lock<std::mutex> mlock(mutex_);
    while (queue_.empty())
    {
      cond_.wait(mlock);
    }
    item = queue_.front();
    queue_.pop();
  }
  void showqueue(){
	  std::unique_lock<std::mutex> mlock(mutex_);
	  //mutex_.lock();
	  for(auto it : queue_){
		  std::cout<< it<<" ";
	  }
	  std::cout<<std::endl;
	  mlock.unlock();
  }
  void push(const T& item)
  {
    std::unique_lock<std::mutex> mlock(mutex_);
    queue_.push_back(item);
    mlock.unlock();
    cond_.notify_one();
  }
  Queue()=default;
  Queue(const Queue&) = delete;            // disable copying
  Queue& operator=(const Queue&) = delete; // disable assignment

 private:
  std::list<T> queue_;
  std::mutex mutex_;
  std::condition_variable cond_;
};

#endif
