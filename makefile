CC = clang
CXX = g++
FLAGS = -Wall -g -w

all:react_server 

react_server: server.cpp cpplib.so
	$(CXX) $(FLAGS) server.cpp ./reactorlib.so -o react_server

cpplib.so: reactor.cpp reactor.hpp
	$(CXX) $(FLAGS) --shared -fPIC reactor.cpp -o reactorlib.so -lpthread

clean:
	rm -f *.o *.so react_server 
