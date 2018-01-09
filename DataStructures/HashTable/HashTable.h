#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "_hashTable.h"
#include <stdbool.h>

HashTable * HashTable_NewTable( void (*payloadDeleter)( HashValue payload ) );
HashTable * HashTable_NewCustomTable( void( *payloadDeleter )( HashValue payload ), 
                                      int ( *HashFunction   )( HashKey key, HashKeyBytes size ), 
                                      int arraySize, float maxLoadFactor );
HashEntry * HashTable_NewEntry( HashKey key, HashKeyBytes keyBytes, HashValue payload );

bool HashTable_DeleteTable( HashTable* hashTable );
bool HashTable_DeleteEntry( HashTable* hashTable, HashEntry* hashEntry );

bool HashTable_KeyExists    ( HashTable* hashTable, HashKey key, HashKeyBytes keyBytes );
bool HashTable_Add          ( HashTable* hashTable, HashKey key, HashKeyBytes keyBytes, HashValue value );
bool HashTable_AddEntry     ( HashTable* hashTable, HashEntry* hashEntry               );
bool HashTable_Remove       ( HashTable* hashTable, HashKey key, HashKeyBytes keyBytes );
bool HashTable_RemoveEntry  ( HashTable* hashTable, HashEntry* hashEntry               );
bool HashTable_ContainsKey  ( HashTable* hashTable, HashKey key, HashKeyBytes keyBytes );
bool HashTable_ContainsValue( HashTable* hashTable, HashValue value                    );
HashValue HashTable_GetValue( HashTable* hashTable, HashKey key, HashKeyBytes keyBytes );

bool HashTable_Clear( HashTable* hashTable );

#endif