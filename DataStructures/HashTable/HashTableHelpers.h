#ifndef HASHTABLEHELPERS_H
#define HASHTABLEHELPERS_H

#include "_hashTable.h"

int HashTable_DefaultHashFunction( HashKey byteArray, HashKeyBytes arraySize );
void HashTable_Rehash( HashTable* hashTable );

#endif