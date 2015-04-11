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

#endif 