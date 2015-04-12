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
#include <string>

//template <typename T>
//using namespace boost;
using namespace std;

class Queue
{
 public:

  string pop()
  {
    unique_lock<mutex> mlock(mutex_);
    while (queue_.empty())
    {
      cond_.wait(mlock);
    }
    auto val = queue_.front();
    queue_.pop_front();
    return val;
  }

  void pop(string& item)
  {
    unique_lock<mutex> mlock(mutex_);
    while (queue_.empty())
    {
      cond_.wait(mlock);
    }
    item = queue_.front();
    queue_.pop_front();
  }
  void showqueue(){
	  unique_lock<mutex> mlock(mutex_);
	  //mutex_.lock();
	  for(auto it : queue_){
		  cout<< it<<" ";
	  }
	  cout<<endl;
	  mlock.unlock();
  }
  void push(const string& item)
  {
    unique_lock<mutex> mlock(mutex_);
    queue_.push_back(item);
    mlock.unlock();
    cond_.notify_one();
  }
  Queue()=default;
  Queue(const Queue&) = delete;            // disable copying
  Queue& operator=(const Queue&) = delete; // disable assignment

 private:
  list<string> queue_;
  mutex mutex_;
  condition_variable cond_;
};

#endif
