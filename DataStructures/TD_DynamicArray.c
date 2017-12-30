
#ifndef TD_DYNAMICARRAY_C
#define TD_DYNAMICARRAY_C

#define _CRTDBG_MAP_ALLOC
#include "_dynamicArray.h"
#include "DynamicArray.h"
#include <stdlib.h>
#include <stdio.h>
#include <crtdbg.h>  
void TD_DynamicArray_Deleter(DynamicArrayPayload integer)
{
    return;
    //free( integer );
}

void TD_DynamicArrayTestDriver()
{
    DynamicArray* dynamicArray = DynamicArray_New( 0, TD_DynamicArray_Deleter );
    DynamicArray_Resize( dynamicArray, 10, 0 );
    for( int i = 0; i < dynamicArray->Count; ++i ) dynamicArray->Payload[i] = i;
    DynamicArray_DebugPrint( dynamicArray );

    DynamicArray_RemoveAt( dynamicArray, dynamicArray->Count-1 );
    DynamicArray_DebugPrint( dynamicArray );

    DynamicArray_RemoveBack( dynamicArray );
    DynamicArray_DebugPrint( dynamicArray );

    DynamicArray_RemoveFront( dynamicArray );
    DynamicArray_DebugPrint( dynamicArray );

    printf("%d", DynamicArray_Find( dynamicArray, 2 ) );
    DynamicArray_Delete( dynamicArray );
    return;
}

#endif