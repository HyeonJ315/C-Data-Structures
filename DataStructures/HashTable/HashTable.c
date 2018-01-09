
#ifndef HASHTABLE_C
#define HASHTABLE_C

#include "HashTable.h"
#include "HashTableHelpers.h"
#include <string.h>
#include "_hashTable.h"
#include <stdlib.h>

HashTable * HashTable_NewTable( void(*payloadDeleter)(HashValue payload) )
{
    HashTable* hashTable = calloc( 1, sizeof(HashTable) );
    if( !hashTable ) return NULL;
    hashTable->PayloadDeleter = payloadDeleter;
    hashTable->HashFunction   = HashTable_DefaultHashFunction;
    hashTable->HashList       = LinkedList_NewList( NULL );
    hashTable->HashArray      = DynamicArray_New ( DEFAULT_HASHARRAY_SIZE, NULL );
    if( !hashTable->HashList   ||
        !hashTable->HashArray )
    {
        if ( hashTable->HashList  ) LinkedList_DeleteList   ( hashTable->HashList  );
        if ( hashTable->HashArray ) DynamicArray_DeleteArray( hashTable->HashArray );
        free( hashTable );
        return NULL;
    }
    hashTable->MaxLoadFactor = DEFAULT_LOADFACTOR;
    return hashTable;
}

HashTable * HashTable_NewCustomTable( void( *payloadDeleter )( HashValue payload ), 
                                      int ( *hashFunction   )( HashKey key, HashKeyBytes size ), 
                                      int arraySize, float maxLoadFactor )
{
    if( arraySize < 0      ) return NULL;
    if( maxLoadFactor <= 0 ) return NULL;
    HashTable* hashTable = calloc(1, sizeof(HashTable));
    if ( !hashTable ) return NULL;
    hashTable->PayloadDeleter = payloadDeleter;
    hashTable->HashFunction   = hashFunction ? hashFunction : HashTable_DefaultHashFunction;
    hashTable->HashList       = LinkedList_NewList(NULL);
    hashTable->HashArray      = DynamicArray_New( arraySize, NULL );
    if (!hashTable->HashList ||
        !hashTable->HashArray )
    {
        if ( hashTable->HashList  ) LinkedList_DeleteList   ( hashTable->HashList  );
        if ( hashTable->HashArray ) DynamicArray_DeleteArray( hashTable->HashArray );
        free( hashTable );
        return NULL;
    }
    hashTable->MaxLoadFactor = maxLoadFactor;
    return hashTable;
}

HashEntry* HashTable_NewEntry( HashKey key, HashKeyBytes keyBytes, HashValue payload )
{
    if ( !key          ) return NULL;
    if ( keyBytes <= 0 ) return NULL;
    HashEntry* hashEntry = malloc( sizeof( HashEntry ) );
    HashKey    hashKey   = malloc( keyBytes );
    if ( !hashEntry || 
         !hashKey )
    {   if( hashEntry ) free( hashEntry );
        if( hashKey   ) free( hashKey   );
        return NULL;
    }
    memcpy( hashKey, key, keyBytes );
    hashEntry->Hash          = -1;
    hashEntry->Key           = hashKey;
    hashEntry->KeyBytes      = keyBytes;
    hashEntry->Value         = payload;
    hashEntry->HashArrayNode = NULL;
    hashEntry->HashListNode  = NULL;
    return hashEntry;
}

bool HashTable_DeleteTable( HashTable* hashTable )
{
    if( !hashTable ) return false;
    int count = hashTable->HashArray->Count;
    LinkedList** linkedLists = (LinkedList**) hashTable->HashArray->Payload;
    for( int i = 0; i < count; ++i )
    {
        // TODO: Deletion.
    }
    return false;
}

bool HashTable_DeleteEntry( HashTable* hashTable, HashEntry* hashEntry )
{
    if ( !hashTable ) return false;
    if ( !hashEntry ) return false;
    if ( hashEntry->Hash >= 0 && hashTable->HashArray->Count > hashEntry->Hash )
    {
        LinkedList* linkedList = (LinkedList*) hashTable->HashArray->Payload[ hashEntry->Hash ];
        LinkedList_NodeRemove( linkedList, hashEntry->HashArrayNode );
    }
    LinkedList_NodeRemove( hashTable->HashList, hashEntry->HashListNode );
    if ( hashTable->PayloadDeleter ) hashTable->PayloadDeleter( hashEntry->Value );
    free( hashEntry->Key );
    free( hashEntry );
    return true;
}

bool HashTable_KeyExists( HashTable* hashTable, HashKey key, HashKeyBytes keyBytes )
{
    return false;
}

bool HashTable_Add( HashTable* hashTable, HashKey key, HashKeyBytes keyBytes, HashValue value )
{
    return false;
}

bool HashTable_AddEntry( HashTable* hashTable, HashEntry* hashEntry )
{
    return false;
}

bool HashTable_Remove( HashTable* hashTable, HashKey key, HashKeyBytes keyBytes )
{
    return false;
}

bool HashTable_RemoveEntry( HashTable* hashTable, HashEntry* hashEntry )
{
    return false;
}

bool HashTable_ContainsKey( HashTable* hashTable, HashKey key, HashKeyBytes keyBytes )
{
    return false;
}

bool HashTable_ContainsValue( HashTable* hashTable, HashValue value )
{
    return false;
}

HashValue HashTable_GetValue( HashTable* hashTable, HashKey key, HashKeyBytes keyBytes )
{
    return NULL;
}

bool HashTable_Clear( HashTable* hashTable )
{
    return false;
}

#endif
