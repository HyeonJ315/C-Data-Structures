#define _CRTDBG_MAP_ALLOC  
#include <crtdbg.h>  
#include "TD_LinkedList.h"
#include "TD_DynamicArray.h"

void main()
{
    TD_LinkedListTestDriver();
    TD_DynamicArray();
    _CrtDumpMemoryLeaks();
}