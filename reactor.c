#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <poll.h>
#include <pthread.h>
#include "hashmap.h"
#include "reactor.h"
#include <unistd.h>

// Function to create a new Reactor
Reactor *createReactor()
{
    Reactor *reactor = (Reactor *)malloc(sizeof(Reactor));
    reactor->fds = NULL;
    reactor->count = 0;
    reactor->isRunning = false;
    reactor->fdMap = createHashMap();
    return reactor;
}
// Function to stop the reactor
void stopReactor(void *reactorPtr)
{
    if (reactorPtr == NULL)
    {
        exit(1);
    }
    Reactor *reactor = (Reactor *)reactorPtr;
    if (reactor->isRunning == false)
    {
        return;
    }
    reactor->isRunning = false;
    pthread_join(reactor->reactorThread, NULL);
}
// Thread function for the reactor
void *reactorThreadFunction(void *reactorPtr)
{
    Reactor *reactor = (Reactor *)reactorPtr;
    while (reactor->isRunning)
    {
        if (reactor->count > 0)
        {
            int numEvents = poll(reactor->fds, reactor->count, -1);
            if (numEvents == -1)
            {
                perror("Error in poll");
                return NULL;
            }

            for (int i = 0; i < reactor->count; i++)
            {

                if (reactor->fds[i].revents & POLLIN)
                {
                    int fd = reactor->fds[i].fd;
                    Node *node = reactor->fdMap->table[fd];
                    handler_t handler = node->funcPtr;
                    handler(fd, reactor);
                }
                else if (reactor->fds[i].revents & POLLHUP || reactor->fds[i].revents & POLLNVAL || reactor->fds[i].revents & POLLERR)
                {
                    removeKey(reactor->fdMap, reactor->fds[i].fd);
                    for (int j = i; j < reactor->count; j++)
                    {
                        reactor->fds[j].fd = reactor->fds[j + 1].fd;
                        reactor->fds[j].events = reactor->fds[j + 1].events;
                        reactor->fds[j].revents = reactor->fds[j + 1].revents;
                    }
                    reactor->fds = (struct pollfd *)realloc(reactor->fds, sizeof(struct pollfd) * (reactor->count -1));
                    reactor->count--;
                    i--;
                }
            }
        }
    }
    return NULL;
}

// Function to start the reactor
void startReactor(void *reactorPtr)
{
    if (reactorPtr == NULL)
    {
        exit(1);
    }
    Reactor *reactor = (Reactor *)reactorPtr;
    // If the reactor is already running or there are no file descriptors
    if (reactor->isRunning == true || reactor->fds == NULL)
    {
        return;
    }
    reactor->isRunning = true;
    pthread_create(&(reactor->reactorThread), NULL, reactorThreadFunction, reactor);

    // Wait for the reactor thread to finish
    pthread_join(reactor->reactorThread, NULL);
}
// Function to add a file descriptor and its associated handler to the reactor
void addFd(void *reactorPtr, int fd, void (*handler)(int, void *))
{
    Reactor *reactor = (Reactor *)reactorPtr;
    if (reactor->fds == NULL)
    {
        reactor->fds = (struct pollfd *)malloc(sizeof(struct pollfd));
    }
    else
    {
        reactor->fds = (struct pollfd *)realloc(reactor->fds, sizeof(struct pollfd) * (reactor->count + 1));
    }

    reactor->fds[reactor->count].fd = fd;
    // Set the events to POLLIN to be alerted when data is ready to be received on this socket
    reactor->fds[reactor->count].events = POLLIN;
    insert(reactor->fdMap, fd, handler);
    reactor->count++;

}
// Function to stop the reactor and free allocated memory
void waitFor(void *reactorPtr)
{
    Reactor *reactor = (Reactor *)reactorPtr;
    stopReactor(reactor);
    free(reactor->fds);
    destroyHashMap(reactor->fdMap);
    free(reactor);
}
