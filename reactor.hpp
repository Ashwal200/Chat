#pragma once

#include <poll.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include<signal.h>

typedef void (*handler_t)(void * , void *);
#define BUF_SIZE 1024
#define CAST_TYPE (void (**)(void * , void *))
#define REACTOR_SIZE sizeof(struct reactor)

typedef struct reactor {
    struct pollfd *pfds; 
    void (**funcs)(void * , void *); 
    pthread_t thread;
    int reactor_size;
} *pReactor;

void *createReactor(); // Create the reactor and allocate space for him
void addFd(void *reactor, int fd , handler_t handler); // Add new client and use the right function
void RemoveHandler(void *reactor, int fd);
void *startReactor(void *reactor); // Put the thread on busy loop
void freeReactor(pReactor pr); // Free all the allocated memory
void WaitFor(void * reactor); // Waits for the thread to terminate
void printData(void *newfd , void * reactor); // Print the data from the client
void acceptClients(void *listener , void * reactor); // Accept any connection from client
void *get_in_addr(struct sockaddr *sa);
void stopReactor(void *); // Stop the thread from running 


