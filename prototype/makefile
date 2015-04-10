dchat: dchat.o User.o ChatNode.o multicast.o util.o parser.o
	g++ -c -o multicast.o multicast.cpp
	gcc -c -o util.o util.c
	g++ -std=c++11 -c -o User.o User.cpp
	g++ -std=c++11 -c -o parser.o parser.cpp
	# parser.o: parser.cpp parser.h ChatNode.h
	# 	g++ -std=c++11 -c parser.cpp
	ChatNode.o: ChatNode.cpp ChatNode.h multicast.h
		g++ -std=c++11 -c ChatNode.cpp
	#g++ -c -o ChatNode.o ChatNode.cpp
	g++ -std=c++11 -c -o dchat.o dchat.cpp
	
	#
	g++ -o dchat dchat.o User.o ChatNode.o multicast.o util.o parser.o

clean:
	rm -f *.o 