# Reactor Project README
This project is a demonstration of the reactor pattern for event handling using C++.

At a high level, a reactor consists of the following components:

1. Event Demultiplexer: 
    This component is responsible for monitoring multiple I/O sources (such as network sockets or file descriptors)
    and determining when any of them are ready for reading or writing.

2. Event Handlers: 
    These are functions or objects that are registered with the reactor to handle specific events. 
    Each event handler is associated with one or more I/O sources and defines the logic to be executed when the
    corresponding events occur.
3. Event Loop: 
    The event loop is the core of the reactor pattern. It continuously listens for events from the event 
    demultiplexer and dispatches them to the appropriate event handlers for processing. 
    It ensures that the code execution remains non-blocking and responsive to events as they occur.


This project uses the POSIX poll system call for event demultiplexing and POSIX threads to handle events concurrently. 
The reactor pattern is implemented in two primary files: reactor.cpp and reactor.hpp. 
The main server application is implemented in server.cpp.

##Getting Started
To build and run the server application, follow the instructions below:

##Prerequisites
You will need a C++ compiler (preferably GCC or similar) that supports the C++11 standard or later. The project also requires the POSIX threads library (pthread). Please ensure these are installed and properly configured on your system.

##Building the project
The project includes a Makefile for easy compilation. Simply run:

##make
This will generate the executable: react_server

##Running the server
After successfully building the project, you can run the server with the following command:


./react_server
The server will start and wait for new connections.


##Cleaning up
To clean up the generated files, you can run:

make clean
This command will delete the built server and client executables, as well as the shared library and object files.

A shared library, also known as a dynamic link library (DLL) in some programming languages, 
is a compiled binary file containing code and data that can be used by multiple programs simultaneously. 
It allows for code reuse and modularity by enabling programs to dynamically link and access the shared library's 
functionality at runtime.