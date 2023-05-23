#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include "reactor.h"


#define MAX_CLIENTS 8000
#define PORT 9034
Reactor *reactor;
void handleClient(int clientSocket, void *reactorPtr)
{
    // Handle client requests here
    // This is just a placeholder function
    // You can replace it with your own logic
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));

    // Receive data from the client
    ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);

    if (bytesRead > 0)
    {
        printf("client \033[38;5;208m id:\033[0m %d, recive: %s",clientSocket, buffer);
    }
    else if (bytesRead == 0)
    {
        // Connection closed by the client
        printf("\033[38;5;208m  client id: %d, disconnected  \033[0m\n",clientSocket);
        close(clientSocket);
    }
    else
    {
        // Error occurred
        perror("Error client");
    }

}

void serverFunc(int fd, void *arg)
{
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);

    Reactor *reactor = (Reactor *)arg;
    if (reactor == NULL)
    {
        perror("error");
        return;
    }

    int client_fd = accept(fd, (struct sockaddr *)&client_addr, &client_len);

    if (client_fd < 0)
    {
        perror("accept() failed");
        return;
    }

    addFd(reactor, client_fd, handleClient);

    // reactor->count++;
    printf("\033[38;5;208mClient connected \033[0m  %s: %d ID:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), client_fd);
    //return arg;
}
void signal_handler() {
     printf("\033[38;5;208m  close all  \033[0m\n");
        int temp;
        for(int i=0 ; i < reactor->count ; i++){
            close(reactor->fds[i].fd);
            temp=reactor->fds[i].fd;
            removeKey(reactor->fdMap,temp);
        }
        destroyHashMap(reactor->fdMap);
        free(reactor->fds);
		free(reactor);
}
int main()
{
    // Create the reactor
    // Reactor *reactor = createReactor();
    
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1)
    {
        perror("Failed to create server socket");
        exit(1);
    }
    int reuse=-1;
if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int)) < 0)
{
    perror("Failed to set SO_REUSEADDR");
    exit(1);
}


    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(PORT);

    if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1)
    {
        perror("Failed to bind server socket");
        exit(1);
    }

    if (listen(serverSocket, MAX_CLIENTS) == -1)
    {
        perror("Failed to listen on server socket");
        exit(1);
    }


     reactor = createReactor();
    addFd(reactor, serverSocket, serverFunc);
    signal(SIGINT, signal_handler);
    // Start the reactor
    startReactor(reactor);

    // Wait for the reactor thread to finish
    waitFor(reactor);

    return 0;
}
