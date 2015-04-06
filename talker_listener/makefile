dchat: dchat.o multicast.o util.o
	g++ -o dchat dchat.o multicast.o util.o
multicast.o: multicast.c util.c
	gcc -c multicast.c util.c
util.o:
	gcc -c util.c

dchat.o: dchat.cpp
	g++ -c dchat.cpp

clean:
	rm -f *.o 