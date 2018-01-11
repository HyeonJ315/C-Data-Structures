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
        HashTable_Add( hashTable, (char*) &value, sizeof(int), (HashValue) value );
    }
    for( int i = 0; i < 256; ++i )
    {
        int value = i;
        printf( "%d", HashTable_ContainsKey( hashTable, (HashKey) &value, sizeof(int) ) );
        printf( "%d", HashTable_ContainsValue( hashTable, (HashValue) value) );
        printf( "_%d  ", (int) HashTable_GetValue( hashTable, (HashKey) &value, 4 ) );
    }
    HashTable_Clear( hashTable );
    HashTable_DeleteTable( hashTable );
}

#endif
