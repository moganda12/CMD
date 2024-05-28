all: main

CXX = g++ -std=c++23
CXXFLAGS = -O3 -ggdb -Wall -Werror

SRCS = src/test.cpp
HEADERS = src/CMD.hpp

main: $(SRCS) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o "$@"

main-debug: $(SRCS) $(HEADERS)
	$(CXX) $(CXXFLAGS) -U_FORTIFY_SOURCE -Og $(SRCS) -o "$@"

clean:
	rm -f main main-debug
