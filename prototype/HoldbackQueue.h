#ifndef HOLDBACKQUEUE_H
#define HOLDBACKQUEUE_H

#include <algorithm>
#include <vector>
#include <map>
#include <string>

using namespace std;

class HoldbackQueue
{
public:
	vector<int> myQueue;
	map<int, string> myMap;

	void enqueue(int val, string msg);

	void dequeue();
	int peek();
	string get(int key);
	bool isEmpty();
	HoldbackQueue();

};

#endif
