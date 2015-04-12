#include "HoldbackQueue.h"

void HoldbackQueue::enqueue(int val, string msg)
{
	myQueue.push_back(-1 * val);
	myMap.insert(make_pair<int, string>(val, msg));
	push_heap(myQueue.begin(),myQueue.end());
}

int HoldbackQueue::peek()
{
	return  -1 * myQueue.front();
}

string HoldbackQueue::get(int key)
{
	return myMap.at(key);
}

void HoldbackQueue::dequeue()
{
	int val = -1 * myQueue.front();
	myMap.erase(val);
	pop_heap(myQueue.begin(),myQueue.end());
	myQueue.pop_back();
}

bool HoldbackQueue::isEmpty()
{
	return myQueue.size() == 0;
}

HoldbackQueue::HoldbackQueue()
{
	
}
