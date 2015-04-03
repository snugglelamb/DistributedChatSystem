dchat: dchat.o User.o ChatNode.o multicast.o util.o
	gcc -c -o multicast.o multicast.c
	g++ -c -o util.o util.cpp
	g++ -c -o ChatNode.o ChatNode.cpp
	g++ -c -o User.o User.cpp
	g++ -c -o dchat.o dchat.cpp
	g++ -o dchat dchat.o User.o ChatNode.o multicast.o util.o

clean:
	rm -f *.o 