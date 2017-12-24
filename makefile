server.out: compileAll
	g++ -pthread -o server.out *.o 
	rm -f *.o

compileAll: include/*.h src/*.cpp
	g++ -Iinclude/ -c src/*.cpp

run:
	./server.out
