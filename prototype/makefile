CC=gcc
CXX=g++
CXXFLAGS= -Wall -std=c++11
dchat: dchat.o User.o ChatNode.o multicast.o parser.o util.o Queue.o HoldbackQueue.o mycomparison.o blocking_priority.o 
	$(CC) -c -o util.o util.c
	$(CXX) $(CXXFLAGS) -c -o mycomparison.o mycomparison.cpp
	$(CXX) $(CXXFLAGS) -c -o blocking_priority.o blocking_priority.cpp
	$(CXX) $(CXXFLAGS) -c -o HoldbackQueue.o HoldbackQueue.cpp
	$(CXX) $(CXXFLAGS) -c -o Queue.o Queue.cpp
	$(CXX) $(CXXFLAGS) -c -o multicast.o multicast.cpp
	$(CXX) $(CXXFLAGS) -c -o User.o User.cpp
	$(CXX) $(CXXFLAGS) -c -o ChatNode.o ChatNode.cpp
	$(CXX) $(CXXFLAGS) -c -o parser.o parser.cpp
	$(CXX) $(CXXFLAGS) -c -o dchat.o dchat.cpp
	$(CXX) $(CXXFLAGS) -o dchat dchat.o User.o ChatNode.o multicast.o parser.o util.o Queue.o HoldbackQueue.o mycomparison.o blocking_priority.o 
clean:
	rm -f *.o  dchat