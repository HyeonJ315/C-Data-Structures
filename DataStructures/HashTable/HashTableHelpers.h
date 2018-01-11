#ifndef HASHTABLEHELPERS_H
#define HASHTABLEHELPERS_H

#include "_hashTable.h"

size_t HashTable_DefaultHashFunction( HashKey byteArray, HashKeyBytes arraySize );
int HashTable_NextPow2(int num);

#endif