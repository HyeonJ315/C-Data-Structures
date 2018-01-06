#ifndef TD_REDBLACKTREE_C
#define TD_REDBLACKTREE_C

#include "../RedBlackTree.h"
#include "../../DynamicArray/DynamicArray.h"

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

char TD_RedBlackTreeComparator( RedBlackTreePayload leftNode, RedBlackTreePayload rightNode )
{
    // Return anything but (true or 1) or (false or 0) for equality.
    if (leftNode == rightNode) return -1;
    return leftNode > rightNode;
}

void TD_RedBlackTreePayloadDeleter( RedBlackTreePayload payload )
{
    //free( payload );
}

void TD_RedBlackTreePrint( RedBlackTree* redBlackTree, RedBlackTreeNode* redBlackTreeNode )
{
    printf("%d ", (int) redBlackTreeNode->Payload );
}

void TD_RedBlackTreeTestDriver()
{
    srand( time(NULL) );
    printf( "Creating a new red black tree.\n" );
    
    DynamicArray* dynamicArray = DynamicArray_New( 0, NULL );

    int R = 1024;
    DynamicArray_Resize(dynamicArray, R, 0);
    int TestArray[20] = {18, 2, 6, 15, 11, 14, 17, 9, 7, 13, 0, 10, 19, 1, 4, 5, 16, 8, 3, 12};
    // Perform stress tests.
    for (int j = 0; j < 100; j++)
    {
        RedBlackTree* redBlackTree = RedBlackTree_NewTree(TD_RedBlackTreeComparator, TD_RedBlackTreePayloadDeleter);
        for (int i = 0; i < R; i++) dynamicArray->Payload[i] = i;
        for (int i = 0; i < R; i++)
        {

            int randIndex = (rand() % (R - i)) + i;
            int tmp = (int)dynamicArray->Payload[i];
            dynamicArray->Payload[i] = dynamicArray->Payload[randIndex];
            dynamicArray->Payload[randIndex] = (DynamicArrayPayload) tmp;
        }
        //printf("Inserting: ");
        for ( int i = 0; i < R; i++ )
        {
            //printf( "%d, ", dynamicArray->Payload[i] );
            RedBlackTree_Insert(redBlackTree, dynamicArray->Payload[i] );
            //printf( "%d, ", TestArray[i] );
            //RedBlackTree_Insert( redBlackTree, TestArray[i] );
        }
        //printf( "\nDeleting: " );
        for ( int i = 0; i < R; i++ )
        {
            //printf("%d, ", dynamicArray->Payload[i] );
            RedBlackTree_NodeRemove( redBlackTree, RedBlackTree_NodeFind( redBlackTree, dynamicArray->Payload[i] ) );
            //printf( "%d, ", TestArray[i] );
            //RedBlackTree_NodeRemove( redBlackTree, RedBlackTree_NodeFind(redBlackTree, TestArray[i]) );
            RedBlackTree_Assert(redBlackTree);
        }
        RedBlackTree_DeleteTree(redBlackTree);
    }
    
    DynamicArray_Delete( dynamicArray );
}

#endif