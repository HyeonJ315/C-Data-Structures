#ifndef DYNAMICARRAY_C
#define DYNAMICARRAY_C

#include "_dynamicArray.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "DynamicArray.h"

DynamicArray* DynamicArray_New( int count, void (*payloadDeleter)(DynamicArrayPayload) )
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
    dynamicArray->Payload        = allocatedPayload;
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
            dynamicArray->PayloadDeleter( dynamicArray->Payload[i] );
        }
    }
    free( dynamicArray->Payload );
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
            dynamicArray->PayloadDeleter(dynamicArray->Payload[i]);
        }
    }

    dynamicArray->Payload = realloc( dynamicArray->Payload, sizeof(DynamicArrayPayload) * count );

    for ( int i = dynamicArray->Count, j = 0; i < count; ++i, ++j )
    {
        dynamicArray->Payload[i] = fillValue;
    }

    dynamicArray->Count = count;
    return true;
}

bool DynamicArray_Add(DynamicArray* dynamicArray, DynamicArrayPayload value )
{
    return DynamicArray_Resize( dynamicArray, 1, value );
}

bool DynamicArray_RemoveAt( DynamicArray* dynamicArray, int index )
{
    if( index < 0 || index >= dynamicArray->Count ) return false;

    DynamicArrayPayload* allocatedPayload = malloc( sizeof(DynamicArrayPayload) * (dynamicArray->Count - 1) );
    if( !allocatedPayload ) return false;

    if (dynamicArray->PayloadDeleter)
        dynamicArray->PayloadDeleter(dynamicArray->Payload[index]);

    for( int i = 0, j = 0; j < dynamicArray->Count; )
    {
        if( j == index )
        {
            j++;
            continue;
        }
        allocatedPayload[i] = dynamicArray->Payload[j];
        i++; j++;
    }
    free( dynamicArray->Payload );
    dynamicArray->Payload = allocatedPayload;
    dynamicArray->Count--;
    return true;
}

bool DynamicArray_RemoveBack( DynamicArray* dynamicArray )
{
    if( dynamicArray->Count <= 0 ) return false;

    if (dynamicArray->PayloadDeleter)
        dynamicArray->PayloadDeleter( dynamicArray->Payload[dynamicArray->Count -1] );

    DynamicArrayPayload* allocatedPayload = realloc( dynamicArray->Payload, sizeof(DynamicArrayPayload) * (dynamicArray->Count-1) );
    if( !allocatedPayload ) return false;

    dynamicArray->Payload = allocatedPayload;
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

    DynamicArrayPayload tmp = dynamicArray->Payload[index1];
    
    dynamicArray->Payload[index1] = dynamicArray->Payload[index2];
    dynamicArray->Payload[index2] = tmp;
    return true;
}

int DynamicArray_Find( DynamicArray* dynamicArray, DynamicArrayPayload value )
{
   for( int i = 0; i < dynamicArray->Count; ++i )
   {
       if( dynamicArray->Payload[i] == value ) return i;
   }
   return -1;
}

void DynamicArray_DebugPrint(DynamicArray * dynamicArray)
{
    printf_s(" Printing dynamic array 0x%p (Count %d)\n", dynamicArray, dynamicArray->Count );
    printf_s("  Payload 0x%p\n", dynamicArray->Payload ); 
    for( int i = 0; i < dynamicArray->Count; ++i )
    {
        printf_s( "    %d: ""0x%p\n", i , dynamicArray->Payload[i] );
    }
}

#endif
