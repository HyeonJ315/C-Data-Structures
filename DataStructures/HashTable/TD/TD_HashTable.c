#ifndef TD_HASHTABLE_C
#define TD_HASHTABLE_C

#include "TD_HashTable.h"
#include "../HashTable.h"
#include <stdio.h>
#include <string.h>
void TD_HashTableTestDriver()
{
    HashTable* hashTable = HashTable_NewTable(NULL);
    for( int i = 0; i < 256; ++i )
    {
        int value = i;
        HashTable_Add( hashTable, (HashKey) &value, sizeof(int), (HashValue) value, true );
    }
    for( int i = 0; i < 256; ++i )
    {
        int value = i;
        HashTable_Add( hashTable, (HashKey)&value, sizeof(int), (HashValue)value, true );
        HashTable_ContainsKey(hashTable, (HashKey)&value, sizeof(int));
        HashTable_ContainsValue(hashTable, (HashValue)value);
        HashTable_GetValue(hashTable, (HashKey)&value, 4);
        HashTable_Remove( hashTable, (HashKey) &value, sizeof(int), true );
        HashTable_ContainsKey( hashTable, (HashKey) &value, sizeof(int) );
        HashTable_ContainsValue( hashTable, (HashValue) value);
        HashTable_GetValue( hashTable, (HashKey) &value, 4 );
    }
    HashTable_Clear( hashTable );
    HashTable_DeleteTable( hashTable );
}

#endif
