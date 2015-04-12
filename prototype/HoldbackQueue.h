#ifndef HOLDBACKQUEUE_H
#define HOLDBACKQUEUE_H

#include <algorithm>
#include <vector>
#include <unordered_map>
#include <string>

using namespace std;

class HoldbackQueue
{
public:
	vector<int> myQueue;
	unordered_map<int, string> myMap;

	void enqueue(int val, string msg);

	void dequeue();
	int peek();
	string get(int key);
	bool isEmpty();
	HoldbackQueue();

};

#endif
