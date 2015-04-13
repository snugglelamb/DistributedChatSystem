dchat: dchat.o User.o ChatNode.o multicast.o parser.o util.o Queue.o HoldbackQueue.o
	gcc -c -o util.o util.c
	g++ -c -o HoldbackQueue.o HoldbackQueue.cpp
	g++ -c -o Queue.o Queue.cpp
	g++ -c -o multicast.o multicast.cpp
	g++ -c -o User.o User.cpp
	g++ -c -o ChatNode.o ChatNode.cpp
	g++ -c -o parser.o parser.cpp
	g++ -c -o dchat.o dchat.cpp
	g++ -o dchat dchat.o User.o ChatNode.o multicast.o parser.o util.o Queue.o HoldbackQueue.o

clean:
	rm -f *.o  
