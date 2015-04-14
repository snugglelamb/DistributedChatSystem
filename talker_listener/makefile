# dchat: dchat.o multicast.o util.o
# 	g++ -o dchat dchat.o multicast.o util.o
# multicast.o: multicast.cpp util.cpp
# 	g++ -c multicast.cpp util.cpp
# util.o: util.cpp
# 	g++ -c util.cpp
# dchat.o: dchat.cpp multicast.cpp
# 	g++ -c dchat.cpp multicast.cpp

dchat: dchat.o multicast.o util.o
	gcc -c -o util.o util.c
	g++ -c -o multicast.o multicast.cpp
	g++ -c -o dchat.o dchat.cpp
	g++ -o dchat dchat.o multicast.o util.o

clean:
	rm -f *.o 