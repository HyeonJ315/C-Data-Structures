#ifndef _HASHTABLE_H
#define _HASHTABLE_H

#include "../DynamicArray/DynamicArray.h"
#include "../LinkedList/LinkedList.h"

#define DEFAULT_HASHARRAY_SIZE 16
#define DEFAULT_LOADFACTOR 1

typedef char* HashKey;
typedef int   HashKeyBytes;
typedef int*  HashValue;

typedef struct _hashEntry
{
    int          Hash;
    HashKey      Key;
    HashKeyBytes KeyBytes;
    HashValue    Value;
    LinkedListNode* HashArrayNode;
    LinkedListNode* HashListNode;
} HashEntry;

typedef struct _hashTable
{
    DynamicArray* HashArray;
    LinkedList* HashList;
    int arraySize;
    int  ( *HashFunction   )( HashKey hashKey, HashKeyBytes keyBytes );
    void ( *PayloadDeleter )( HashValue payload );
    float LoadFactor;
    float MaxLoadFactor;
    int Count;
} HashTable;

#endif