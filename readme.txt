# Reactor Project README
This project is a demonstration of the reactor pattern for event handling using C++. The reactor pattern is an event handling pattern for handling service requests delivered concurrently to a service handler by one or more inputs. The service handler then demultiplexes the incoming requests and dispatches them synchronously to the associated request handlers.

This project uses the POSIX poll system call for event demultiplexing and POSIX threads to handle events concurrently. The reactor pattern is implemented in two primary files: reactor.cpp and reactor.hpp. The main server application is implemented in server.cpp.

##Getting Started
To build and run the server application, follow the instructions below.

##Prerequisites
You will need a C++ compiler (preferably GCC or similar) that supports the C++11 standard or later. The project also requires the POSIX threads library (pthread). Please ensure these are installed and properly configured on your system.

##Building the project
The project includes a Makefile for easy compilation. Simply run:

##make
This will generate two executables: react_server and client.

##Running the server
After successfully building the project, you can run the server with the following command:


./react_server
The server will start and wait for new connections.

##Running the client
To run the client, use the following command:

./client
Functionality
Here's a brief description of the functionality provided in the reactor.cpp and reactor.hpp files:

###createReactor: This function initializes and returns a new Reactor instance.

###addFd: This function adds a file descriptor and its corresponding handler function to the Reactor instance.

###RemoveHandler: This function removes the handler for a specific file descriptor in the Reactor instance.

###startReactor: This function starts the Reactor's event loop.

###delReactor: This function deletes the Reactor and frees all its resources.

###WaitFor: This function waits for the Reactor's thread to finish.

###printData: This function prints data received from a socket to the console.

###acceptClients: This function accepts new client connections and adds them to the Reactor instance.

###get_in_addr: This function returns the address of a socket, whether it's IPv4 or IPv6.

###The server.cpp file implements the server logic. It creates a new Reactor, sets up a listening socket, and adds it to the Reactor with acceptClients as the handler function.

##Cleaning up
To clean up the generated files, you can run:


###make clean
This command will delete the built server and client executables, as well as the shared library and object files.

##Contributing
We welcome contributions! Please submit a pull request with any enhancements or bug fixes. All contributions should follow the existing coding style and conventions.

##License
This project is licensed under the MIT License. See the LICENSE file for details.
