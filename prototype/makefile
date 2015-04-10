
dchat: dchat.o User.o ChatNode.o multicast.o parser.o
	g++ -c -o multicast.o multicast.cpp
	g++ -c -o User.o User.cpp
	g++ -c -o ChatNode.o ChatNode.cpp
	g++ -c -o parser.o parser.cpp
	g++ -c -o dchar.o dchat.cpp
	g++ -o dchat dchat.o User.o ChatNode.o multicast.o parser.o


clean:
	rm -f *.o 