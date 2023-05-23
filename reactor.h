#ifndef REACTOR_H
#define REACTOR_H

#include <stdbool.h>
#include <poll.h>
#include <pthread.h>
#include "hashmap.h"

typedef void (*handler_t)(int fd, void *arg);

// Structure to represent a Reactor
typedef struct Reactor {
    struct pollfd *fds;      // Array of file descriptors to monitor
    int count;               // Number of file descriptors in the array
    bool isRunning;          // Flag to indicate if the reactor is running
    HashMap *fdMap;          // Hash map to store file descriptors and their associated handlers
    pthread_t reactorThread; // Thread for the reactor
} Reactor;

Reactor* createReactor();
void stopReactor(void *reactorPtr);
void startReactor(void *reactorPtr);
void addFd(void *reactorPtr, int fd, handler_t handler);
void waitFor(void *reactorPtr);

#endif /* REACTOR_H */
