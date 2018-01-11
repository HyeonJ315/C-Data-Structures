#ifndef _DYNAMICARRAY_H
#define _DYNAMICARRAY_H


typedef void* DynamicArrayPayload;

typedef struct _dynamicArray
{
    int Count;
    DynamicArrayPayload* PayloadArr;
    void (*PayloadDeleter)(DynamicArrayPayload);
} DynamicArray;

#endif