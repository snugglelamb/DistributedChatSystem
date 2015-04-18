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
 private:
  priority_queue<Req,vector<Req>,mycomparison> pq;
  mutex mutex_;
  condition_variable cond_;
};
#endif BLOCK_PQ
