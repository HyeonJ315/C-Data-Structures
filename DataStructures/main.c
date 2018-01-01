#define _CRTDBG_MAP_ALLOC  
#include <crtdbg.h>  
#include "TD_LinkedList.h"
#include "TD_DynamicArray.h"
#include "TD_RedBlackTree.h"

void main()
{
    //TD_LinkedListTestDriver();
    //TD_DynamicArrayTestDriver();
    TD_RedBlackTreeTestDriver();
    _CrtDumpMemoryLeaks();
    return;
}