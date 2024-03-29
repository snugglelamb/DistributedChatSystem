
#ifndef CONCURRENT_QUEUE_
#define CONCURRENT_QUEUE_

#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <list>
#include <iostream>
#include <string>
#include <cstdlib>
//template <typename T>

//using namespace boost;
using namespace std;

class Queue
{
 public:

  string pop();

  void pop(string& item);
//  void showqueue(){
//	  unique_lock<mutex> mlock(mutex_);
//	  //mutex_.lock();
//	  for(auto it : queue_){
//		  cout<< it<<" ";
//	  }
//	  cout<<endl;
//	  mlock.unlock();
//  }
  void push(const string& item);

  Queue()=default;
  Queue(const Queue&) = delete;            // disable copying
  Queue& operator=(const Queue&) = delete; // disable assignment

 private:
  list<string> queue_;
  mutex mutex_;
  condition_variable cond_;
};

#endif
