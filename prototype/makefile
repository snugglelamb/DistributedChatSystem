dchat: dchat.o User.o ChatNode.o multicast.o parser.o util.o HoldbackQueue.o
	gcc -c -o util.o util.c
	g++-4.7 -O2 -Wall --std=c++11 -c -o HoldbackQueue.o HoldbackQueue.cpp
	g++-4.7 -O2 -Wall --std=c++11 -c -o multicast.o multicast.cpp
	g++-4.7 -O2 -Wall --std=c++11 -c -o User.o User.cpp
	g++-4.7 -O2 -Wall --std=c++11 -c -o ChatNode.o ChatNode.cpp
	g++-4.7 -O2 -Wall --std=c++11 -c -o parser.o parser.cpp
	g++-4.7 -O2 -Wall --std=c++11 -c -o dchat.o dchat.cpp
	g++-4.7 -O2 -Wall --std=c++11 -o dchat dchat.o User.o ChatNode.o multicast.o parser.o util.o HoldbackQueue.o

clean:
	rm -f *.o  