#include "HoldbackQueue.h"

void HoldbackQueue::enqueue(int val)
{
	queue.push_back(-1 * val);
	push_heap(queue.begin(),queue.end());
}

int HoldbackQueue::peek()
{
	return -1 * queue.front();
}

void HoldbackQueue::dequeue()
{
	pop_heap(queue.begin(),queue.end());
	queue.pop_back();
}


HoldbackQueue::HoldbackQueue()
{
	
}