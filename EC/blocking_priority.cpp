#include "blocking_priority.h"

  Block_PQ::Block_PQ(){
	  current = 0;
	  totalQ = 0;
  }
  
  string Block_PQ::pop()
  {
    unique_lock<mutex> mlock(mutex_);
	// waiting for producer
    while (vec_fairPQ.empty() && totalQ == 0)
    {
	  // printf("FLAG: in POP waiting\n");
      cond_.wait(mlock);
	  current = 0;
    }
	//round robin
	if (totalQ > 1) {
		cout << "Now totalQ in vector equals: " << totalQ << endl;
		cout << "Current Queue No:" << current << endl;
	}
	string cnt;
	// pop 1 msg for each user 
	if (! vec_fairPQ[current].empty()){
		cnt = vec_fairPQ[current].top().request;
		vec_fairPQ[current].pop();
	} 
	if (current != (totalQ - 1)){
		current++;
	} else {
		// check and erase empty obj
		int i = 0;
		vector<FairPQ>::iterator it = vec_fairPQ.begin();
		for (i=0; i<totalQ; i++) {
			if ( vec_fairPQ[i].empty()) {
				vec_fairPQ.erase(it+i);
				i--;
				totalQ--;
			}
		} //end for
		// reset to top
		current = 0;		
		// cout << "FLAG: Finish Cleanup." << endl;
		// cout << "totalQ: " << totalQ << "; current: "<< current << ";status of Q: "<< vec_fairPQ.size()<< endl;
	}

    return cnt;
  }



  void Block_PQ::push(const Req& item)
  {
	  bool found = false;
      unique_lock<mutex> mlock(mutex_);
	  
	  if (totalQ == 0) {
		  // push new Req in vector
		  vec_fairPQ.push_back(FairPQ());
		  vec_fairPQ[0].push(item);
		  totalQ = 1;
	  } else {
		  // check if label exists in vec_fairPQ
		  int i = 0;
		for (i = 0; i< totalQ; i++) {
			if ( vec_fairPQ[i].top().label == item.label) {
				 vec_fairPQ[i].push(item);
				found = true;
				break;
			}
		} // end for
		if (found) {
			
		} else {
  		  // push new Req in vector
  		  vec_fairPQ.push_back(FairPQ());
  		  vec_fairPQ[totalQ].push(item);
  		  totalQ++;
		}	
	}

    mlock.unlock();
    cond_.notify_one();

  }



