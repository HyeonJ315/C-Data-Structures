#ifndef HASHTABLEHELPERS_H
#define HASHTABLEHELPERS_H

#include "_hashTable.h"

size_t HashTable_DefaultHashFunction( HashKey byteArray, HashKeyBytes arraySize );
size_t HashTable_NextPow2(size_t num);
size_t HashTable_PrevPow2(size_t num);
#endif