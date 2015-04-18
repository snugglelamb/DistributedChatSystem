
#include "Queue.h"

  string Queue::pop()
  {
    unique_lock<mutex> mlock(mutex_);
    while (queue_.empty())
    {
      cond_.wait(mlock);
    }

    int nt = rand()%queue_.size();
    auto it = next(queue_.begin() , nt);
    queue_.erase(it);
//    auto val = queue_.front();
//    queue_.pop_front();
    return *it;
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
//  void showqueue(){
//	  unique_lock<mutex> mlock(mutex_);
//	  //mutex_.lock();
//	  for(auto it : queue_){
//		  cout<< it<<" ";
//	  }
//	  cout<<endl;
//	  mlock.unlock();
//  }
  void Queue::push(const string& item)
  {
    
    unique_lock<mutex> mlock(mutex_);
    queue_.push_back(item);
    mlock.unlock();
    cond_.notify_one();
  }



