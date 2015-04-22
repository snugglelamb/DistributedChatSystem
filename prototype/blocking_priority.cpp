
#include "blocking_priority.h"

  string Block_PQ::pop()
  {
    unique_lock<mutex> mlock(mutex_);
    //
    while (pq.empty())
    {
      cond_.wait(mlock);
    }

    string cnt = pq.top().request;
    pq.pop();
    return cnt;
  }



  void Block_PQ::push(const Req& item)
  {

    unique_lock<mutex> mlock(mutex_);
    pq.push(item);
    mlock.unlock();
    cond_.notify_one();
  }



