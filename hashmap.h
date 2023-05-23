#ifndef HASHMAP_H
#define HASHMAP_H

#define SIZE 500

typedef void *(*funcPtr)(int fd, void *arg);

// Structure to represent a hash map node
typedef struct Node
{
    int key;                            // Key associated with the node
    void (*funcPtr)(int fd, void *arg); // Function pointer stored in the node
} Node;

// Structure to represent the hash map
typedef struct HashMap
{
    Node *table[SIZE]; // Array to store the hash map elements
} HashMap;

HashMap *createHashMap();
int hash(int key);
void insert(HashMap *hashMap, int key, void (*funcPtr)(int fd, void *arg));
void display(HashMap *hashMap);
void destroyHashMap(HashMap *hashMap);
void removeKey(HashMap *hashMap, int key);

#endif /* HASHMAP_H */
