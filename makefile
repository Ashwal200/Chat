CC = gcc
CXX = g++
FLAGS = -Wall -g -w

all:pollServer pollClient

pollServer: pollserver.cpp cpplib.so
	$(CXX) $(FLAGS) pollserver.cpp ./cpplib.so -o pollServer

pollClient: pollclient.cpp cpplib.so
	$(CXX) $(FLAGS) pollclient.cpp ./cpplib.so -o pollClient

# shared library for all the c++ code files
cpplib.so: reactor.cpp reactor.hpp
	$(CXX) $(FLAGS) --shared -fPIC reactor.cpp -o cpplib.so -lpthread


clean:
	rm -f *.o *.so pollServer pollClient
