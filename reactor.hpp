#ifndef DESIGN_PATTERNS_REACTOR_HPP
#define DESIGN_PATTERNS_REACTOR_HPP

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
    struct pollfd *pfds; // list of fds to listen on
    void (**funcs)(void * , void *); // list of handlers function
    pthread_t thread; // the thread who run the handlers
    int reactor_size;
    ///Every fd in the pfds[i] get handle by a function from funcs[i]
} *pReactor;

void *createReactor();
void addFd(void *reactor, int fd , handler_t handler);
void RemoveHandler(void *reactor, int fd);
void *startReactor(void *reactor);
void delReactor(pReactor pr); /// delete and free al the reactor
// no stop 
void WaitFor(void * reactor);

void printData(void *newfd , void * reactor);
void acceptClients(void *listener , void * reactor);
void *get_in_addr(struct sockaddr *sa);


#endif //DESIGN_PATTERNS_REACTOR_HPP
