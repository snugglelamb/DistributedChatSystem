#ifndef HOLDBACKQUEUE_H
#define HOLDBACKQUEUE_H

#include <algorithm>
#include <vector>

using namespace std;

class HoldbackQueue
{
public:
	vector<int> queue;

	void enqueue(int val);
	void dequeue();
	int peek();

	HoldbackQueue();

};

<<<<<<< HEAD
#endif
=======
#endif 
>>>>>>> b37b774f9d5d7726503eb8c8b177fe5e9d8e790f
