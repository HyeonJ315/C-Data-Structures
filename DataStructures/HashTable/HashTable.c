#ifndef HASHTABLE_C
#define HASHTABLE_C

#include "HashTable.h"
#include "HashTableHelpers.h"
#include "_hashTable.h"
#include <string.h>
#include <stdlib.h>

HashTable * HashTable_NewTable( void(*payloadDeleter)(HashValue payload) )
{
    HashTable* hashTable = malloc( sizeof(HashTable) );
    if( !hashTable ) return NULL;

    hashTable->PayloadDeleter = payloadDeleter;
    hashTable->HashFunction   = HashTable_DefaultHashFunction;
    hashTable->LoadFactor     = 0;
    hashTable->MaxLoadFactor  = DEFAULT_LOADFACTOR;
    hashTable->Count          = 0;
    hashTable->HashList       = LinkedList_NewList( NULL );
    hashTable->HashArray      = DynamicArray_NewArray( DEFAULT_HASHARRAY_SIZE, NULL );

    if( !hashTable->HashList   ||
        !hashTable->HashArray )
    {
        if ( hashTable->HashList  ) LinkedList_DeleteList   ( hashTable->HashList  );
        if ( hashTable->HashArray ) DynamicArray_DeleteArray( hashTable->HashArray );
        free( hashTable );
        return NULL;
    }
    return hashTable;
}

HashTable * HashTable_NewCustomTable( void  ( *payloadDeleter )( HashValue payload ), 
                                      size_t( *hashFunction   )( HashKey key, HashKeyBytes size ), 
                                      int arraySize, float maxLoadFactor )
{
    if( arraySize <= 0     ) return NULL;
    if( maxLoadFactor <= 0 ) return NULL;
    arraySize = HashTable_NextPow2( arraySize );
    HashTable* hashTable = malloc( sizeof(HashTable) );
    if( !hashTable ) return NULL;

    hashTable->PayloadDeleter = payloadDeleter;
    hashTable->HashFunction   = hashFunction ? hashFunction : HashTable_DefaultHashFunction;
    hashTable->LoadFactor     = 0;
    hashTable->MaxLoadFactor  = maxLoadFactor;
    hashTable->Count          = 0;
    hashTable->HashList       = LinkedList_NewList( NULL );
    hashTable->HashArray      = DynamicArray_NewArray( arraySize, NULL );

    if( !hashTable->HashList   ||
        !hashTable->HashArray )
    {
        if ( hashTable->HashList  ) LinkedList_DeleteList   ( hashTable->HashList  );
        if ( hashTable->HashArray ) DynamicArray_DeleteArray( hashTable->HashArray );
        free( hashTable );
        return NULL;
    }
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
    hashEntry->Hash             = -1;
    hashEntry->Key              = hashKey;
    hashEntry->KeyBytes         = keyBytes;
    hashEntry->Value            = payload;
    hashEntry->HashArrayListRef = NULL;
    hashEntry->HashArrayNode    = NULL;
    hashEntry->HashListNode     = NULL;

    return hashEntry;
}

bool HashTable_DeleteTable( HashTable* hashTable )
{
    if( !hashTable ) return false;
    LinkedList** linkedLists = (LinkedList**) hashTable->HashArray->PayloadArr;
    int count = hashTable->HashArray->Count;

    for( int i = 0; i < count; ++i )
    {
        LinkedList* linkedList = linkedLists[i];
        if( linkedList && linkedList->Count > 0 )
        {
            LinkedListNode * currNode = linkedList->Head;
            LinkedListNode * headNode = linkedList->Head;
            do
            {
                HashTable_DeleteEntry( hashTable, (HashEntry*) currNode->Payload );
                currNode = currNode->Next;
            } while( currNode != headNode );
            LinkedList_DeleteList( linkedList );
        }
    }
    DynamicArray_DeleteArray( hashTable->HashArray );
    LinkedList_DeleteList( hashTable->HashList );
    free( hashTable );
    return false;
}

bool HashTable_DeleteEntry( HashTable* hashTable, HashEntry* hashEntry )
{
    if ( !hashTable ) return false;
    if ( !hashEntry ) return false;
    if ( hashTable->PayloadDeleter ) hashTable->PayloadDeleter( hashEntry->Value );
    free( hashEntry->Key );
    free( hashEntry );
    return true;
}

void HashTable_Rehash( HashTable* hashTable, size_t arraySize )
{
    if ( !hashTable            ) return;
    if ( !hashTable->HashList  ) return;
    if ( !hashTable->HashArray ) return;
    
    int nextPow2 = HashTable_NextPow2( arraySize );
    for( int i = 0; i < hashTable->HashArray->Count; ++i )
    {
        LinkedList* linkedList = (LinkedList*) hashTable->HashArray->PayloadArr[i];
        if( linkedList ) LinkedList_DeleteList( (LinkedList*) hashTable->HashArray->PayloadArr[i] );
        hashTable->HashArray->PayloadArr[i] = NULL;
    }
    DynamicArray_Resize( hashTable->HashArray, nextPow2 - hashTable->HashArray->Count, NULL );
    if (hashTable->HashList->Count > 0) 
    {
        LinkedListNode* currNode = hashTable->HashList->Head;
        LinkedListNode* headNode = hashTable->HashList->Head;
        do
        {
            HashEntry* hashEntry = (HashEntry*)currNode->Payload;
            currNode = currNode->Next;
            int hashIndex = hashEntry->Hash % hashTable->HashArray->Count;
            LinkedList* linkedList = (LinkedList*) hashTable->HashArray->PayloadArr[hashIndex];
            if ( !linkedList )
            {
                linkedList = LinkedList_NewList(NULL);
                hashTable->HashArray->PayloadArr[hashIndex] = (DynamicArrayPayload) linkedList;
            }
            LinkedList_AppendTail( linkedList, (LinkedListPayload) hashEntry );
            hashEntry->HashArrayListRef = linkedList;
            hashEntry->HashArrayNode    = linkedList->Tail;
        } while (currNode != headNode);
    }
    hashTable->LoadFactor = (float) hashTable->HashList->Count / hashTable->HashArray->Count;
}

bool HashTable_Add( HashTable* hashTable, HashKey key, HashKeyBytes keyBytes, HashValue value )
{
    HashEntry* hashEntry = HashTable_NewEntry( key, keyBytes, value );
    hashEntry->Hash = hashTable->HashFunction( hashEntry->Key, hashEntry->KeyBytes );
    if( !HashTable_AddEntry(hashTable, hashEntry) )
    {
        HashTable_DeleteEntry( hashTable, hashEntry );
        return false;
    }
    return true;
}

bool HashTable_AddEntry( HashTable* hashTable, HashEntry* hashEntry )
{
    if ( !hashTable ) return false;
    if ( !hashEntry ) return false;
    if ( !hashTable->HashList  ) return false;
    if ( !hashTable->HashArray ) return false;
    if( HashTable_ContainsKey( hashTable, hashEntry->Key, hashEntry->KeyBytes ) ) return false;

    if( !LinkedList_AppendTail( hashTable->HashList, (LinkedListPayload) hashEntry ) ) return false;

    int hashIndex = hashEntry->Hash % hashTable->HashArray->Count;
    if ( !hashTable->HashArray->PayloadArr[hashIndex] )
        hashTable->HashArray->PayloadArr[hashIndex] = (DynamicArrayPayload) LinkedList_NewList(NULL);

    LinkedList* linkedList = (LinkedList*) hashTable->HashArray->PayloadArr[hashIndex];
    if ( !LinkedList_AppendTail( linkedList, (LinkedListPayload) hashEntry ) )
    {
        LinkedList_NodeRemove( hashTable->HashList, hashTable->HashList->Tail );
        return false;
    }

    hashEntry->HashArrayListRef = linkedList;
    hashEntry->HashArrayNode    = linkedList->Tail;
    hashEntry->HashListNode     = hashTable->HashList->Tail;
    hashTable->LoadFactor       = (float) hashTable->HashList->Count / hashTable->HashArray->Count;
    hashTable->Count++;
    if( hashTable->LoadFactor > hashTable->MaxLoadFactor ) HashTable_Rehash( hashTable, hashTable->HashArray->Count );
    return true;
}

bool HashTable_Remove( HashTable* hashTable, HashKey key, HashKeyBytes keyBytes )
{
    if ( !hashTable    ) return false;
    if ( keyBytes <= 0 ) return false;
    HashEntry* hashEntry = HashTable_GetEntry( hashTable, key, keyBytes );
    if( !hashEntry ) return false;
    return HashTable_RemoveEntry( hashTable, hashEntry );
}

bool HashTable_RemoveEntry( HashTable* hashTable, HashEntry* hashEntry )
{
    if ( !hashTable ) return false;
    if ( !hashEntry ) return false;
    LinkedList_NodeRemove( hashEntry->HashArrayListRef, hashEntry->HashArrayNode );
    LinkedList_NodeRemove( hashTable->HashList, hashEntry->HashListNode          );
    if( hashEntry->HashArrayListRef->Count == 0 )
    {
        int hashIndex = hashEntry->Hash % hashTable->HashArray->Count;
        hashTable->HashArray->PayloadArr[hashIndex] = NULL;
        LinkedList_DeleteList( hashEntry->HashArrayListRef );
    }
    HashTable_DeleteEntry( hashTable, hashEntry );
    hashTable->Count--;
    hashTable->LoadFactor = (float) hashTable->HashList->Count / hashTable->HashArray->Count;
    return true;
}

bool HashTable_ContainsKey( HashTable* hashTable, HashKey key, HashKeyBytes keyBytes )
{
    size_t hashIndex = hashTable->HashFunction(key, keyBytes) % hashTable->HashArray->Count;
    LinkedList* linkedList = (LinkedList*) hashTable->HashArray->PayloadArr[hashIndex];
    if (!linkedList) return false;
    LinkedListNode* currNode = linkedList->Head;
    LinkedListNode* headNode = linkedList->Head;
    do
    {
        HashEntry* hashEntry = (HashEntry*)currNode->Payload;
        currNode = currNode->Next;
        if (keyBytes != hashEntry->KeyBytes) continue;
        int i = 0;
        for (; i < keyBytes; ++i)
            if (hashEntry->Key[i] != key[i]) break;
        if (i == keyBytes) return true;
    } while (currNode != headNode);
    return false;
}

bool HashTable_ContainsValue( HashTable* hashTable, HashValue value )
{
    if ( !hashTable ) return false;
    if ( !hashTable->HashList ) return false;
    if ( hashTable->HashList->Count <= 0 ) return false;
    LinkedListNode* currNode = hashTable->HashList->Head;
    LinkedListNode* headNode = hashTable->HashList->Head;
    do
    {
        HashEntry* hashEntry = (HashEntry*)currNode->Payload;
        currNode = currNode->Next;
        if( value == hashEntry->Value ) return true;
    } while (currNode != headNode);
    return false;
}

HashEntry* HashTable_GetEntry( HashTable* hashTable, HashKey key, HashKeyBytes keyBytes )
{
    int hashIndex = hashTable->HashFunction(key, keyBytes) % hashTable->HashArray->Count;
    LinkedList* linkedList = (LinkedList*)hashTable->HashArray->PayloadArr[hashIndex];
    if (!linkedList) return NULL;
    LinkedListNode* currNode = linkedList->Head;
    LinkedListNode* headNode = linkedList->Head;
    do
    {
        HashEntry* hashEntry = (HashEntry*)currNode->Payload;
        currNode = currNode->Next;
        if (keyBytes != hashEntry->KeyBytes) continue;
        int i = 0;
        for (; i < keyBytes; ++i)
            if (hashEntry->Key[i] != key[i]) break;
        if (i == keyBytes) return hashEntry;
    } while (currNode != headNode);
    return NULL;
}

HashValue HashTable_GetValue( HashTable* hashTable, HashKey key, HashKeyBytes keyBytes )
{
    int hashIndex = hashTable->HashFunction(key, keyBytes) % hashTable->HashArray->Count;
    LinkedList* linkedList = (LinkedList*)hashTable->HashArray->PayloadArr[hashIndex];
    if (!linkedList) return NULL;
    LinkedListNode* currNode = linkedList->Head;
    LinkedListNode* headNode = linkedList->Head;
    do
    {
        HashEntry* hashEntry = (HashEntry*)currNode->Payload;
        currNode = currNode->Next;
        if (keyBytes != hashEntry->KeyBytes) continue;
        int i = 0;
        for (; i < keyBytes; ++i)
            if (hashEntry->Key[i] != key[i]) break;
        if (i == keyBytes) return hashEntry->Value;
    } while (currNode != headNode);
    return NULL;
}

bool HashTable_Clear( HashTable* hashTable )
{
    if ( !hashTable ) return false;
    if ( !hashTable->HashList ) return false;
    while ( hashTable->Count > 0 )
        HashTable_RemoveEntry(hashTable, hashTable->HashList->Head->Payload );
    hashTable->LoadFactor = 0;
    return true;
}

#endif
