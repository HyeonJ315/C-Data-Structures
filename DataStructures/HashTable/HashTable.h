#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "_hashTable.h"
#include <stdbool.h>

HashTable * HashTable_NewTable( void (*payloadDeleter)( HashValue payload ) );
HashTable * HashTable_NewCustomTable( void  ( *payloadDeleter )( HashValue payload ), 
                                      size_t( *HashFunction   )( HashKey key, HashKeyBytes size ), 
                                      int arraySize, float maxLoadFactor );
HashEntry * HashTable_NewEntry( HashKey key, HashKeyBytes keyBytes, HashValue payload );

bool HashTable_DeleteTable( HashTable* hashTable );
bool HashTable_DeleteEntry( HashTable* hashTable, HashEntry* hashEntry );

void HashTable_Rehash       ( HashTable* hashTable, size_t arraySize );
bool HashTable_Add          ( HashTable* hashTable, HashKey key, HashKeyBytes keyBytes, HashValue value, bool rehashOnMaxLoad );
bool HashTable_AddEntry     ( HashTable* hashTable, HashEntry* hashEntry, bool rehashOnMaxLoad );
bool HashTable_Remove       ( HashTable* hashTable, HashKey key, HashKeyBytes keyBytes, bool rehashOnMinLoad );
bool HashTable_RemoveEntry  ( HashTable* hashTable, HashEntry* hashEntry, bool rehashOnMinLoad );
bool HashTable_ContainsKey  ( HashTable* hashTable, HashKey key, HashKeyBytes keyBytes );
bool HashTable_ContainsValue( HashTable* hashTable, HashValue value                    );
HashValue  HashTable_GetValue( HashTable* hashTable, HashKey key, HashKeyBytes keyBytes );
HashEntry* HashTable_GetEntry( HashTable* hashTable, HashKey key, HashKeyBytes keyBytes );
bool HashTable_Clear( HashTable* hashTable );

#endif