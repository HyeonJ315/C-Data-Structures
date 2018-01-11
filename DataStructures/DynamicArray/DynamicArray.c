#ifndef DYNAMICARRAY_C
#define DYNAMICARRAY_C

#include "_dynamicArray.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "DynamicArray.h"

DynamicArray* DynamicArray_NewArray( int count, void (*payloadDeleter)(DynamicArrayPayload) )
{
    void* allocatedContainer   = calloc( 1, sizeof( DynamicArray ) );
    DynamicArrayPayload* allocatedPayload = calloc( 1, sizeof(DynamicArrayPayload) * count   );
    if ( !allocatedContainer || !allocatedPayload )
    {
        if ( allocatedContainer ) free( allocatedContainer );
        if ( allocatedPayload   ) free( allocatedPayload   );
        return NULL;
    }
    DynamicArray* dynamicArray   = allocatedContainer;
    dynamicArray->PayloadArr        = allocatedPayload;
    dynamicArray->PayloadDeleter = payloadDeleter;
    dynamicArray->Count          = count;
    return dynamicArray;
}

void DynamicArray_DeleteArray( DynamicArray* dynamicArray )
{
    if ( !dynamicArray ) return;
    if ( dynamicArray->PayloadDeleter )
    {
        for (int i = 0; i < dynamicArray->Count; ++i)
        {
            dynamicArray->PayloadDeleter( dynamicArray->PayloadArr[i] );
        }
    }
    free( dynamicArray->PayloadArr );
    free( dynamicArray          );
}

bool DynamicArray_Resize( DynamicArray* dynamicArray, int valueCount, DynamicArrayPayload fillValue )
{
    if ( !dynamicArray  ) return false;
    int count = valueCount + dynamicArray->Count;
    if ( count < 0 ) count = 0;
    if ( dynamicArray->PayloadDeleter )
    {
        for (int i = count; i < dynamicArray->Count; ++i)
        {
            dynamicArray->PayloadDeleter(dynamicArray->PayloadArr[i]);
        }
    }

    dynamicArray->PayloadArr = realloc( dynamicArray->PayloadArr, sizeof(DynamicArrayPayload) * count );

    for ( int i = dynamicArray->Count, j = 0; i < count; ++i, ++j )
    {
        dynamicArray->PayloadArr[i] = fillValue;
    }

    dynamicArray->Count = count;
    return true;
}

bool DynamicArray_Add(DynamicArray* dynamicArray, DynamicArrayPayload value )
{
    return DynamicArray_Resize( dynamicArray, 1, value );
}

bool DynamicArray_RemoveAt( DynamicArray* dynamicArray, size_t index )
{
    if( index < 0 || index >= dynamicArray->Count ) return false;

    DynamicArrayPayload* allocatedPayload = malloc( sizeof(DynamicArrayPayload) * (dynamicArray->Count - 1) );
    if( !allocatedPayload ) return false;

    if (dynamicArray->PayloadDeleter) dynamicArray->PayloadDeleter(dynamicArray->PayloadArr[index]);

    memcpy( &allocatedPayload[0], &dynamicArray->PayloadArr[0], index * sizeof(DynamicArrayPayload) );
    memcpy( &allocatedPayload[index], &dynamicArray->PayloadArr[index+1], (dynamicArray->Count-index-1) * sizeof(DynamicArrayPayload) ); 
    free( dynamicArray->PayloadArr );
    dynamicArray->PayloadArr = allocatedPayload;
    dynamicArray->Count--;
    return true;
}

bool DynamicArray_RemoveBack( DynamicArray* dynamicArray )
{
    if( dynamicArray->Count <= 0 ) return false;

    if (dynamicArray->PayloadDeleter)
        dynamicArray->PayloadDeleter( dynamicArray->PayloadArr[dynamicArray->Count -1] );

    DynamicArrayPayload* allocatedPayload = realloc( dynamicArray->PayloadArr, sizeof(DynamicArrayPayload) * (dynamicArray->Count-1) );
    if( !allocatedPayload ) return false;

    dynamicArray->PayloadArr = allocatedPayload;
    dynamicArray->Count--;
    return true;
}

bool DynamicArray_RemoveFront( DynamicArray* dynamicArray )
{
    return DynamicArray_RemoveAt( dynamicArray, 0 );
}

bool DynamicArray_Swap( DynamicArray* dynamicArray, int index1, int index2 )
{
    if (index1 < 0 || index1 >= dynamicArray->Count) return false;
    if (index2 < 0 || index2 >= dynamicArray->Count) return false;

    DynamicArrayPayload tmp = dynamicArray->PayloadArr[index1];
    
    dynamicArray->PayloadArr[index1] = dynamicArray->PayloadArr[index2];
    dynamicArray->PayloadArr[index2] = tmp;
    return true;
}

int DynamicArray_Find( DynamicArray* dynamicArray, DynamicArrayPayload value )
{
   for( int i = 0; i < dynamicArray->Count; ++i )
   {
       if( dynamicArray->PayloadArr[i] == value ) return i;
   }
   return -1;
}

void DynamicArray_DebugPrint(DynamicArray * dynamicArray)
{
    printf_s(" Printing dynamic array 0x%p (Count %d)\n", dynamicArray, dynamicArray->Count );
    printf_s("  PayloadArr 0x%p\n", dynamicArray->PayloadArr ); 
    for( int i = 0; i < dynamicArray->Count; ++i )
    {
        printf_s( "    %d: ""0x%p\n", i , dynamicArray->PayloadArr[i] );
    }
}

#endif
