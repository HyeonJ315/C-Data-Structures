#ifndef DYNAMICARRAY_H
#define DYNAMICARRAY_H

#include "_dynamicArray.h"
#include <stddef.h>
#include <stdbool.h>

DynamicArray* DynamicArray_NewArray   ( int count, void (*payloadDeleter)(DynamicArrayPayload)                  );
void          DynamicArray_DeleteArray( DynamicArray* dynamicArray                                              );
bool          DynamicArray_Resize     ( DynamicArray* dynamicArray, int valueCount, DynamicArrayPayload fillVal );
bool          DynamicArray_Add        ( DynamicArray* dynamicArray, DynamicArrayPayload value                   );
bool          DynamicArray_RemoveAt   ( DynamicArray* dynamicArray, size_t index                                );
bool          DynamicArray_RemoveBack ( DynamicArray* dynamicArray                                              );
bool          DynamicArray_RemoveFront( DynamicArray* dynamicArray                                              );
bool          DynamicArray_Swap       ( DynamicArray* dynamicArray, int index1, int index2                      );
int           DynamicArray_Find       ( DynamicArray* dynamicArray, DynamicArrayPayload value                   );
void          DynamicArray_DebugPrint ( DynamicArray* dynamicArray                                              );

#endif