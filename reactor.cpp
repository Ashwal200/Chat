#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "reactor.hpp"



void *createReactor() {
    pReactor reactor = (pReactor) malloc(REACTOR_SIZE);
    if (reactor == NULL) {
        perror("reactor malloc error");
        exit(1);
    }
    reactor->pfds = (struct pollfd *) malloc(sizeof(struct pollfd));
    if (reactor->pfds == NULL) {
        perror("reactor malloc");
        exit(1);
    }
    reactor->funcs = CAST_TYPE malloc(8);
    if (reactor->funcs == NULL) {
        perror("reactor malloc error");
        exit(1);
    }
    reactor->reactor_size = 0;
    return reactor;
}

void addFd(void *reactor, int fd , handler_t handler) {
    pReactor preactor = (pReactor) reactor;

    if (preactor->reactor_size == 0) 
    {
        preactor->pfds[0].fd = fd; // Adding the fd to the fd list
        preactor->pfds[0].events = POLLIN; // Set the "event listener" to input

        preactor->funcs[0] = handler; // Adding the function to the handler list
        preactor->reactor_size += 1;
        // since its the first handler installation we need to create the handle thread
        pthread_create(&preactor->thread, NULL, startReactor, reactor);
    } 
    else 
    {
        preactor->reactor_size += 1;
        preactor->pfds = (struct pollfd *) realloc(preactor->pfds, sizeof(struct pollfd) * preactor->reactor_size);
        if (preactor->pfds == NULL) {
            perror("reactor malloc error");
            exit(1);
        }
        preactor->pfds[preactor->reactor_size - 1].fd = fd; // adding the fd to the fd list
        preactor->pfds[preactor->reactor_size - 1].events = POLLIN; // set the "event listener" to input

        preactor->funcs = CAST_TYPE realloc(preactor->funcs, 8 * preactor->reactor_size);
        if (preactor->funcs == NULL) {
            perror("reactor malloc error");
            exit(1);
        }
        preactor->funcs[preactor->reactor_size - 1] = handler; // adding the function to the handler list

    }
}

void RemoveHandler(void *reactor, int fd) {
    pReactor preactor = (pReactor) reactor;
    int fd_index = -1;
    for (int i = 0; i < preactor->reactor_size; ++i) 
    {
        if (preactor->pfds[i].fd == fd) {
            fd_index = i;
            break;
        }
    }  
    if (fd_index == -1) 
    { // in case the fd doesn't exist
        return;
    }
    else 
    {
        --(preactor->reactor_size);
        struct pollfd *newpfds = (struct pollfd *) malloc(sizeof(struct pollfd) * preactor->reactor_size);
        void (**newfuncs)(void *, void *) = CAST_TYPE malloc(8 * preactor->reactor_size);

        if (fd_index != 0) {
            memcpy(newpfds, preactor->pfds, fd_index * sizeof(struct pollfd)); // copy everything BEFORE the index
            memcpy(newfuncs, preactor->funcs, fd_index * 8); // copy everything BEFORE the index
        }

        if (fd_index != preactor->reactor_size) {
            memcpy(newpfds + fd_index, preactor->pfds + fd_index + 1,
                   (preactor->reactor_size - fd_index) * sizeof(struct pollfd)); // copy everything AFTER the index
            memcpy(newfuncs + fd_index, preactor->funcs + fd_index + 1,
                   (preactor->reactor_size - fd_index) * sizeof(struct pollfd)); // copy everything AFTER the index
        }
        free(preactor->funcs);
        free(preactor->pfds);
        preactor->pfds = newpfds;
        preactor->funcs = newfuncs;
    }
}

void freeReactor(pReactor preactor){
    if (preactor->reactor_size != 0){
        free(preactor->pfds);
        free(preactor->funcs);
    }
    free(preactor);
}


void WaitFor(void * reactor)
{
    pReactor preactor = (pReactor) reactor;
    pthread_join(preactor->thread, NULL);

}



void printData(void *newfd, void * reactor) 
{
    int* fd = (int*) newfd;
    
    char buff[BUF_SIZE] ={0};
    int bytes = read(*fd, buff, BUF_SIZE);
    if (bytes < 0) {
        perror("read");
    }
    else if (bytes == 0){
        printf("server: client disconnected... \n");
        RemoveHandler(reactor, *fd);
        close(*fd);
    }
    else 
    {
        printf("New message: %s\n", buff);
    }
}

void acceptClients(void *listener , void * reactor) {
    int* fd = (int*) listener;
    pReactor preactor = (pReactor) reactor;
    int newfd; // Newly accept()ed socket descriptor
    struct sockaddr_storage remoteaddr; // Client address
    char remoteIP[INET6_ADDRSTRLEN];
    socklen_t addrlen = sizeof(remoteaddr);

    newfd = accept(*fd, (struct sockaddr *) &remoteaddr, &addrlen);

    if (newfd == -1) 
    {
        perror("accept");
    } else 
    {
        handler_t handler = printData;
        addFd(preactor, newfd, handler);

        printf("server: new connection from %s on socket %d\n",
               inet_ntop(remoteaddr.ss_family, get_in_addr((struct sockaddr *) &remoteaddr), remoteIP,
                         INET6_ADDRSTRLEN), newfd);
    }
}
// Get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in *) sa)->sin_addr);
    }

    return &(((struct sockaddr_in6 *) sa)->sin6_addr);
}

void stopReactor(void * reactor)
{
    pReactor preactor = (pReactor) reactor;
    pthread_cancel(preactor->thread);
}

void *startReactor(void *reactor) {
    pReactor preactor = (pReactor) reactor;

    
    while (1) {
        poll(preactor->pfds, preactor->reactor_size, -1); // -1 mean making the poll wait forever (with no timeout)
        for (int i = 0; i < preactor->reactor_size; ++i) { // for all the fds
            if (preactor->pfds[i].revents & POLLIN) { // if the events is one we read from the fd
                preactor->funcs[i](&(preactor->pfds[i].fd) , preactor); // run the suitable function[i] on fd[i]
            }
        }
    }
}