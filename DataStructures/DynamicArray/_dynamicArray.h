#ifndef _DYNAMICARRAY_H
#define _DYNAMICARRAY_H


typedef int* DynamicArrayPayload;

typedef struct _dynamicArray
{
    int Count;
    DynamicArrayPayload* Payload;
    void (*PayloadDeleter)(DynamicArrayPayload);
} DynamicArray;

#endif