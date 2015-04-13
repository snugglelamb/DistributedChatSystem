dchat: dchat.o User.o ChatNode.o multicast.o parser.o util.o Queue.o HoldbackQueue.o
	gcc -c -o util.o util.c
	g++ -std=c++11 -c -o HoldbackQueue.o HoldbackQueue.cpp
	g++ -std=c++11 -c -o Queue.o Queue.cpp
	g++ -std=c++11 -c -o multicast.o multicast.cpp
	g++ -std=c++11 -c -o User.o User.cpp
	g++ -std=c++11 -c -o ChatNode.o ChatNode.cpp
	g++ -std=c++11 -c -o parser.o parser.cpp
	g++ -std=c++11 -c -o dchat.o dchat.cpp
	g++ -std=c++11 -o dchat dchat.o User.o ChatNode.o multicast.o parser.o util.o Queue.o HoldbackQueue.o
clean:
	rm -f *.o  