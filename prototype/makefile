dchat: dchat.o User.o ChatNode.o multicast.o util.o
	gcc -c -o multicast.o multicast.c
	gcc -c -o util.o util.c
	g++ -c -o ChatNode.o ChatNode.cpp 
	g++ -c -o User.o User.cpp 
	g++ -c -o dchat.o dchat.cpp
	g++ -c -o parser.o parser.cpp
	g++ -o dchat dchat.o User.o ChatNode.o multicast.o util.o parser.o

clean:
	rm -f *.o 