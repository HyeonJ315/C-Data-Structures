#ifndef HASHTABLEHELPERS_C
#define HASHTABLEHELPERS_C

#include "HashTableHelpers.h"
#include <stddef.h>

size_t HashTable_DefaultHashFunction( HashKey byteArray, HashKeyBytes arraySize)
{
    size_t hash = 0;
    for ( int i = 0; i < arraySize; ++i )
        hash = (31*hash) + byteArray[i];
    return hash;
}

size_t HashTable_NextPow2( size_t num )
{
    size_t pow2 = 1;
    while ( pow2 <= num ) pow2 <<= 1;
    return pow2;
}

size_t HashTable_PrevPow2( size_t num )
{
    size_t pow2 = 1 << 31;
    while( pow2 >= num ) pow2 >>= 1;
    return pow2;
}
#endif
