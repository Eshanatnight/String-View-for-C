CFLAGS=-Wall -Wextra -std=c11 -pedantic -ggdb
CXXFLAGS=-Wall -Wextra -std=c++17 -pedantic -ggdb

.PHONY: all
all: test-c test-cxx
	./test-c
	./test-cxx

test-c: test.c sv.c sv.h
	$(CC) $(CFLAGS) -o test-c test.c

test-cxx: test.c sv.c sv.h
<<<<<<< HEAD
	$(CXX) $(CXXFLAGS) -x c++ -o test-cxx test/test.c
=======
	$(CXX) $(CXXFLAGS) -x c++ -o test-cxx test/test.c
>>>>>>> 36c24fe356493dd2fdc6fe71a34e0ade05dc1ff4
