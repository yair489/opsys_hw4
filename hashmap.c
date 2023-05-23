#include <stdio.h>
#include <stdlib.h>
#include "hashmap.h"

// Function to create a new hash map
HashMap *createHashMap()
{
    HashMap *hashMap = (HashMap *)malloc(sizeof(HashMap));
    // Initialize each element in the hash map's table as NULL
    for (int i = 0; i < SIZE; i++)
    {
        hashMap->table[i] = NULL;
    }
    return hashMap;
}

// Hash function to determine the index for a given key
int hash(int key)
{
    return key % SIZE;
}

// Function to insert a key-value pair into the hash map
void insert(HashMap *hashMap, int key, void (*funcPtr)(int fd, void *arg))
{
    int index = hash(key);
    // Create a new node
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->key = key;
    newNode->funcPtr = funcPtr;
    // Insert the node at the calculated index in the hash map's table
    hashMap->table[index] = newNode;
}

// Function to remove a key from the hash map
void removeKey(HashMap *hashMap, int key)
{
    int index = hash(key);
    if (hashMap->table[index] != NULL)
    {
        // Free the memory allocated for the node and set the element to NULL
        free(hashMap->table[index]);
        hashMap->table[index] = NULL;
    }
}
// Function to display the contents of the hash map
void display(HashMap *hashMap)
{
    for (int i = 0; i < SIZE; i++)
    {
        if (hashMap->table[i] != NULL)
        {
            // Print the index and key of the node
            printf("Index %d: Key: %d\n", i, hashMap->table[i]->key);
        }
    }
}
// Function to destroy the hash map and free allocated memory
void destroyHashMap(HashMap *hashMap)
{
    for (int i = 0; i < SIZE; i++)
    {
        if (hashMap->table[i] != NULL)
        {
            // Free the memory allocated for each node and set the element to NULL
            free(hashMap->table[i]);
            hashMap->table[i] = NULL;
        }
    }
    // Free the memory allocated for the hash map itself
    free(hashMap);
}
