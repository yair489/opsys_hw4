st_# Reactor Pattern - Assignment 4
## Yair Turgeman - 
## Shoval Zohar  - 

In this assignment, we have carried out on you, we have financed the reactor design template, which is a mechanism that accepts a client's fileDescriptor and a function to execute
when the fd is "hot". The reactor will support receiving a large number of fd and will run in a single trad. If there is no input, we will wait until received
The message from each of the fds that are waiting for them, and will perform the appropriate function for the fd. 
The name of the library is called st_reactor.so.

The reactor only supports reading.
According to the DESIGN PATTERN of the REACTOR
We implemented a data structure that maps between the FD and the function,in addition we implemented selector mechanism, which is responsible for the poll that listens to all the FD and performs the operation listed in the reactor's table.
Since the selector listens to multiple FDs, and it is possible that more than one will be "hot" at a certain stage, in such a case order
The treatment does not matter.

### The REACTOR will support the following functions:
A.

void* createReactor()

which creates a reactor.
Returns a pointer to the reactor structure that will be passed to the following functions. When the reactor is created it does not work but every structure
(init and alloc). The data will be collected

B.

Void stopReactor(void * this)

Stops the reactor if it is active. otherwise does nothing.


C.

void startReactor(void * this)

The trouble of a reactor begins.
The nuisance will live in a busy loop and will actually call poll.

D.

Void addFd (void * this, int fd, handler_t handler). 

Where t_handler is a pointer to a function that will be called when the fd is hot.
We use t_handler.

E.

void WaitFor(void * this)

Wait for join_pthread (3) until the reactor thread is finished.


## The cover of the library - the application
The server works exactly like Beej's chat server (selectserver for example) the client that is in the model and works in front of Beej will work in front of him.
We used port 8000.



### In addition we have MakeFile that build all the necessary files and the main programs:
make all

### Before we run the server we need to export shared libraries:
export LD_LIBRARY_PATH="."

### The running format of the server is:
./react_server

### After we need to open the Terminal windows and write :
telent 127.0.0.1 9034
  
  
and than send messages like chat Beej ...