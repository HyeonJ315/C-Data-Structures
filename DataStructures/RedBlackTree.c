#ifndef REDBLACKTREE_C
#define REDBLACKTREE_C

#include "RedBlackTree.h"
#include "LinkedList.h"
#include <stdlib.h>
#include <stdbool.h>

RedBlackTree* RedBlackTree_NewTree( bool (*comparator)( RedBlackTreePayload,RedBlackTreePayload ), 
                                    void (*payloadDeleter)(RedBlackTreePayload ) )
{
    if ( !comparator ) return NULL;
    RedBlackTree* redBlackTree = calloc( 1, sizeof(RedBlackTree) );
    if( !redBlackTree ) return NULL;
    redBlackTree->LeftGreaterThanRight = comparator;
    redBlackTree->PayloadDeleter       = payloadDeleter;
    return redBlackTree;
}

RedBlackTreeNode* RedBlackTree_NewNode( RedBlackTreePayload payload )
{
    RedBlackTreeNode* redBlackTreeNode = calloc( 1, sizeof(RedBlackTreeNode) );
    if( !redBlackTreeNode ) return NULL;
    redBlackTreeNode->Payload = payload;
    redBlackTreeNode->Color   = Red;
    return redBlackTreeNode;
}

void RedBlackTree_DeleteTree( RedBlackTree* redBlackTree )
{
    RedBlackTree_InOrderTraversal( redBlackTree, RedBlackTree_DeleteNode );
    free( redBlackTree );
}

void RedBlackTree_DeleteNode( RedBlackTree* redBlackTree, RedBlackTreeNode* redBlackTreeNode )
{
    if( redBlackTree->PayloadDeleter )
    {
        redBlackTree->PayloadDeleter( redBlackTreeNode->Payload ); 
    }
    free( redBlackTreeNode );
}

bool RedBlackTree_InsertNode( RedBlackTree* redBlackTree, RedBlackTreePayload payload )
{
    RedBlackTreeNode* newNode = RedBlackTree_NewNode( payload );

    if( redBlackTree->Count == 0 ) 
    {
        redBlackTree->Root  = newNode;
        redBlackTree->Count = 1;
        newNode->Color = Black;
        return true;
    }

    RedBlackTreeNode* uncleNode       = NULL;
    RedBlackTreeNode* grandParentNode = NULL;
    RedBlackTreeNode* parentNode      = NULL;
    bool comparatorPrev = false;;
    bool comparatorCurr = false;
    RedBlackTreeNode* currentNode = redBlackTree->Root;
    while( currentNode != NULL )
    {
        grandParentNode = parentNode;
        parentNode      = currentNode;
        comparatorPrev  = comparatorCurr;
        comparatorCurr  = redBlackTree->LeftGreaterThanRight( payload, currentNode->Payload );
        if( parentNode      ) currentNode = comparatorCurr ? currentNode->RightChild     : currentNode->LeftChild;
        if( grandParentNode ) uncleNode   = comparatorPrev ? grandParentNode->LeftChild  : grandParentNode->RightChild; 
    }

    if (comparatorCurr) parentNode->RightChild = newNode; 
    else                parentNode->LeftChild  = newNode;
    newNode->Parent = parentNode;

    if ( !(grandParentNode && uncleNode) ) return true;

    // Case 1, recolor node.
    if ( uncleNode->Color == Red )
    {
        RedBlackTree_RecolorNode( parentNode      );
        RedBlackTree_RecolorNode( grandParentNode );
        RedBlackTree_RecolorNode( uncleNode       );
        redBlackTree->Count++;
        return true;
    }

    bool triangleShaped = comparatorPrev ^ comparatorCurr;
    // Case 2 triangle shaped
    if ( triangleShaped )
    {
        if( comparatorCurr ) RedBlackTree_SingleLeftRotation ( parentNode );
        else                 RedBlackTree_SingleRightRotation( parentNode );
    }
    // Case 3 line shaped
    else
    {
        if( comparatorCurr ) RedBlackTree_DoubleLeftRotation ( grandParentNode );
        else                 RedBlackTree_DoubleRightRotation( grandParentNode );
    }
    redBlackTree->Count++;
    return true;
}

bool RedBlackTree_RecolorNode        ( RedBlackTreeNode* redBlackTreeNode )
{
    if( !redBlackTreeNode ) return false;
    if (redBlackTreeNode->Color == Red) redBlackTreeNode->Color = Black;
    else                                redBlackTreeNode->Color = Red;
    return true;
}

bool RedBlackTree_SingleLeftRotation ( RedBlackTreeNode* redBlackTreeNode )
{
    if ( !redBlackTreeNode             ) return false;
    if ( !redBlackTreeNode->RightChild ) return false;
    RedBlackTreeNode* parentNode  = redBlackTreeNode;
    RedBlackTreeNode* currentNode = redBlackTreeNode->RightChild;
    currentNode->Parent     = parentNode->Parent;
    currentNode->LeftChild  = parentNode;
    parentNode->Parent      = currentNode;
    parentNode->RightChild  = NULL;
    return true;
}

bool RedBlackTree_SingleRightRotation( RedBlackTreeNode* redBlackTreeNode )
{
    if ( !redBlackTreeNode             ) return false;
    if ( !redBlackTreeNode->LeftChild  ) return false;
    RedBlackTreeNode* parentNode  = redBlackTreeNode;
    RedBlackTreeNode* currentNode = redBlackTreeNode->LeftChild;
    currentNode->Parent     = parentNode->Parent;
    currentNode->RightChild = parentNode;
    parentNode->Parent      = currentNode;
    parentNode->LeftChild   = NULL;
    return true;
}

bool RedBlackTree_DoubleLeftRotation ( RedBlackTreeNode* redBlackTreeNode )
{
    if ( !redBlackTreeNode                        ) return false;
    if ( !redBlackTreeNode->RightChild            ) return false;
    if ( !redBlackTreeNode->RightChild->LeftChild ) return false;
    RedBlackTreeNode* grandParentNode = redBlackTreeNode;
    RedBlackTreeNode* parentNode      = redBlackTreeNode->RightChild;
    RedBlackTreeNode* currentNode     = redBlackTreeNode->RightChild->LeftChild;
    parentNode->Parent          = grandParentNode->Parent;
    parentNode->LeftChild       = grandParentNode;
    grandParentNode->Parent     = parentNode;
    grandParentNode->RightChild = currentNode;
    currentNode->Parent         = grandParentNode;
    RedBlackTree_RecolorNode( parentNode      );
    RedBlackTree_RecolorNode( grandParentNode );
    return true;
}

bool RedBlackTree_DoubleRightRotation( RedBlackTreeNode* redBlackTreeNode )
{
    if ( !redBlackTreeNode                        ) return false;
    if ( !redBlackTreeNode->LeftChild             ) return false;
    if ( !redBlackTreeNode->LeftChild->RightChild ) return false;
    RedBlackTreeNode* grandParentNode = redBlackTreeNode;
    RedBlackTreeNode* parentNode      = redBlackTreeNode->LeftChild;
    RedBlackTreeNode* currentNode     = redBlackTreeNode->LeftChild->RightChild;
    parentNode->Parent          = grandParentNode->Parent;
    parentNode->RightChild      = grandParentNode;
    grandParentNode->Parent     = parentNode;
    grandParentNode->LeftChild  = currentNode;
    currentNode->Parent         = grandParentNode;
    RedBlackTree_RecolorNode( parentNode      );
    RedBlackTree_RecolorNode( grandParentNode );
    return false;
}

void RedBlackTree_PreOrderTraversal ( RedBlackTree* redBlackTree, void (*forEachNode)(RedBlackTree*, RedBlackTreeNode*) )
{
    LinkedList* linkedList = LinkedList_NewList(NULL);
    if( !redBlackTree->Root ) return;
    LinkedList_AppendHeadNode( linkedList, (LinkedListPayload) redBlackTree->Root );
    while ( linkedList->Count != 0 )
    {
        RedBlackTreeNode* currentNode = (RedBlackTreeNode*) linkedList->Head->Payload;
        if( forEachNode ) forEachNode( redBlackTree, currentNode );
        LinkedList_RemoveNode( linkedList, linkedList->Head );
        if( currentNode->RightChild ) LinkedList_AppendHeadNode( linkedList, (LinkedListPayload) currentNode->RightChild );
        if( currentNode->LeftChild  ) LinkedList_AppendHeadNode( linkedList, (LinkedListPayload) currentNode->LeftChild  );
    }
}

void RedBlackTree_InOrderTraversal  ( RedBlackTree* redBlackTree, void (*forEachNode)(RedBlackTree*, RedBlackTreeNode*) )
{
    LinkedList* linkedList = LinkedList_NewList(NULL);
    RedBlackTreeNode* currentNode = redBlackTree->Root;
    while (linkedList->Count != 0 || currentNode != NULL)
    {
        if (currentNode != NULL)
        {
            LinkedList_AppendHeadNode( linkedList, (LinkedListPayload) currentNode );
            currentNode = currentNode->LeftChild;
        }
        else
        {
            currentNode = (RedBlackTreeNode*) linkedList->Head->Payload;
            RedBlackTreeNode* currentNode_rightChild = currentNode->RightChild;
            LinkedList_RemoveNode(linkedList, linkedList->Head);
            if( forEachNode ) forEachNode( redBlackTree, currentNode );
            currentNode = currentNode_rightChild;
        }
    }
}

void RedBlackTree_PostOrderTraversal( RedBlackTree* redBlackTree, void (*forEachNode)(RedBlackTree*, RedBlackTreeNode*) )
{
    LinkedList* linkedList = LinkedList_NewList(NULL);
    if ( !redBlackTree->Root ) return;
    RedBlackTreeNode* currentNode = redBlackTree->Root;
    do
    {
        while( currentNode )
        {
            if( currentNode->RightChild ) LinkedList_AppendHeadNode( linkedList, (LinkedListPayload) currentNode->RightChild );
            LinkedList_AppendHeadNode( linkedList, (LinkedListPayload) currentNode );
            currentNode = currentNode->LeftChild;
        }
        currentNode = (RedBlackTreeNode*) linkedList->Head->Payload;
        LinkedList_RemoveNode( linkedList, linkedList->Head );
        if( currentNode->RightChild && (RedBlackTreeNode*) linkedList->Head->Payload == currentNode->RightChild )
        {
            LinkedList_RemoveNode( linkedList, linkedList->Head );
            LinkedList_AppendHeadNode( linkedList, (LinkedListPayload) currentNode );
            currentNode = currentNode->RightChild;
        }
        else
        {
            if( forEachNode ) forEachNode( redBlackTree, currentNode );
            currentNode = NULL;
        }
    } while( linkedList->Count != 0 );
}

#endif /* REDBLACKTREE_C */