tests: test.o newtest.o
	g++ test.o -o tests -L/usr/local/lib -lgtest -lgtest_main -pthread
	g++ newtest.o -o newtests -L/usr/local/lib -lgtest -lgtest_main -pthread

program: main.o
	g++ main.o -o program

main.o: main.cpp dynamicarray.hpp linkedlist.hpp exceptions.hpp 
	g++ -std=c++17 -stdlib=libc++ main.cpp -c

test.o: test.cpp dynamicarray.hpp linkedlist.hpp arraysequence.hpp listsequence.hpp
	g++ -std=c++17 -stdlib=libc++ -I/usr/local/include test.cpp -c

newtest.o: newtest.cpp dequelistsequence.hpp queuelistsequence.hpp stacklistsequence.hpp listsequence.hpp
	g++ -std=c++17 -stdlib=libc++ -I/usr/local/include newtest.cpp -c

clean:
	rm -f tests program *.o

run:
	./program

test:
	./tests

newtest:
	./newtests
