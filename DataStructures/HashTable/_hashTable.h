#ifndef _HASHTABLE_H
#define _HASHTABLE_H

#include "../DynamicArray/DynamicArray.h"
#include "../LinkedList/LinkedList.h"

#define DEFAULT_HASHARRAY_SIZE 16
#define DEFAULT_LOADFACTOR 0.75

typedef char* HashKey;
typedef int   HashKeyBytes;
typedef void* HashValue;

typedef struct _hashEntry
{
    size_t       Hash;
    HashKey      Key;
    HashKeyBytes KeyBytes;
    HashValue    Value;

    LinkedList*     HashArrayListRef;
    LinkedListNode* HashArrayNode;
    LinkedListNode* HashListNode;
} HashEntry;

typedef struct _hashTable
{
    DynamicArray* HashArray;
    LinkedList* HashList;
    size_t ( *HashFunction   )( HashKey hashKey, HashKeyBytes keyBytes );
    void   ( *PayloadDeleter )( HashValue payload );
    float LoadFactor;
    float MaxLoadFactor;
    int Count;
} HashTable;

#endif