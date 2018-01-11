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

void TD_Randomizer( DynamicArray* dynamicArray )
{
    int count = dynamicArray->Count;
    for (int i = 0; i < count; i++)
    {
        int randIndex = (rand() % (count - i)) + i;
        int tmp = (int)dynamicArray->PayloadArr[i];
        dynamicArray->PayloadArr[i] = dynamicArray->PayloadArr[randIndex];
        dynamicArray->PayloadArr[randIndex] = (DynamicArrayPayload)tmp;
    }
}

void TD_RedBlackTreeTestDriver()
{
    srand( time(NULL) );
    printf( "Creating a new red black tree.\n" );
    
    DynamicArray* dynamicArray = DynamicArray_NewArray( 0, NULL );

    int R = 20;
    DynamicArray_Resize(dynamicArray, R, 0);
    // Perform stress tests.
    for (int j = 0; j < 10; j++)
    {
        RedBlackTree* redBlackTree = RedBlackTree_NewTree(TD_RedBlackTreeComparator, TD_RedBlackTreePayloadDeleter);
        for (int i = 0; i < R; i++) dynamicArray->PayloadArr[i] = i;

        TD_Randomizer( dynamicArray );
        printf( "\nInserting: " );
        for ( int i = 0; i < R; i++ )
        {
            printf( "%d, ", dynamicArray->PayloadArr[i] );
            RedBlackTree_Insert(redBlackTree, dynamicArray->PayloadArr[i] );
        }
        TD_Randomizer( dynamicArray );
        printf( "\nDeleting: " );
        for ( int i = 0; i < R; i++ )
        {
            printf("%d, ", dynamicArray->PayloadArr[i] );
            RedBlackTree_NodeRemove( redBlackTree, RedBlackTree_NodeFind( redBlackTree, dynamicArray->PayloadArr[i] ) );
            RedBlackTree_Assert( redBlackTree, true );
        }
        RedBlackTree_DeleteTree(redBlackTree);
    }
    
    DynamicArray_DeleteArray( dynamicArray );
}

#endif