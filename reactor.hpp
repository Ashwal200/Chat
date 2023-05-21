#ifndef DESIGN_PATTERNS_REACTOR_HPP
#define DESIGN_PATTERNS_REACTOR_HPP

#include <poll.h>
#include <pthread.h>
#include <unistd.h>
typedef void (*handler_t)(int *);
#define cast_Handler (void (*)(void *))
#define REACTOR_SIZE sizeof(struct reactor)

typedef struct reactor {
    struct pollfd *pfds; // list of fds to listen on
    void (**funcs)(void *); // list of handlers function
    pthread_t thread; // the thread who run the handlers
    int handlers_size;
    ///Every fd in the pfds[i] get handle by a function from funcs[i]
} *pReactor;

void *createReactor();
void addFd(void *reactor, int fd , handler_t handler);
void RemoveHandler(void *reactor, int fd);
void *activate_poll_listener(void *reactor);
void delReactor(pReactor pr); /// delete and free al the reactor

void WaitFor(void * reactor);

#endif //DESIGN_PATTERNS_REACTOR_HPP
