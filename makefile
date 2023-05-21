CC = gcc
CXX = g++
FLAGS = -Wall -g -w

all:server client

server: server.cpp cpplib.so
	$(CXX) $(FLAGS) server.cpp ./cpplib.so -o server

client: client.cpp cpplib.so
	$(CXX) $(FLAGS) client.cpp ./cpplib.so -o client

# shared library for all the c++ code files
cpplib.so: reactor.cpp reactor.hpp
	$(CXX) $(FLAGS) --shared -fPIC reactor.cpp -o cpplib.so -lpthread


clean:
	rm -f *.o *.so server client
