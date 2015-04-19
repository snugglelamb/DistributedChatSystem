#ifndef BLOCK_PQ
#define BLOCK_PQ
#include <string>
#include <mutex>
#include <thread>
#include <queue>
#include <vector>
#include <condition_variable>
#include "mycomparison.h"
#include "Req.h"
using namespace std;
class Block_PQ{
public:
  string pop();
  void push(const Req& item);
  Block_PQ()=default;


 private:
  priority_queue<Req,vector<Req>,mycomparison> pq;
  Block_PQ& operator=(const Block_PQ&) ; // disable assignment
  Block_PQ(const Block_PQ&);            // disable copying
  mutex mutex_;
  condition_variable cond_;
};

#endif

