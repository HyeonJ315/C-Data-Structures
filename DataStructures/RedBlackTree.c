#ifndef REDBLACKTREE_C
#define REDBLACKTREE_C

#include "RedBlackTree.h"
#include "RedBlackTreeHelpers.h"
#include "LinkedList.h"

#include <stdlib.h>
#include <stdbool.h>

RedBlackTree* RedBlackTree_NewTree( char (*comparator)( RedBlackTreePayload,RedBlackTreePayload ), 
                                    void (*payloadDeleter)( RedBlackTreePayload ) )
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
    if( !redBlackTree ) return;
    RedBlackTree_PostOrderTraversal( redBlackTree, RedBlackTree_DeleteNode );
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

bool RedBlackTree_Insert( RedBlackTree* redBlackTree, RedBlackTreePayload payload )
{
    RedBlackTreeNode* newNode = RedBlackTree_NewNode( payload );
    if( !newNode ) return false;
    return RedBlackTree_NodeInsert( redBlackTree, newNode );
}

bool RedBlackTree_NodeInsert( RedBlackTree* redBlackTree, RedBlackTreeNode* newNode )
{
    RedBlackTreeNode* parentNode = NULL;
    RedBlackTreeNode* currentNode = redBlackTree->Root;
    bool comparatorCurr = false;
    while (currentNode != NULL)
    {
        parentNode = currentNode;
        comparatorCurr = redBlackTree->LeftGreaterThanRight( newNode->Payload, currentNode->Payload );
        currentNode = comparatorCurr ? currentNode->RightChild : currentNode->LeftChild;
    }
    if (parentNode)
    {
        if (comparatorCurr) parentNode->RightChild = newNode;
        else                parentNode->LeftChild  = newNode;
    }
    newNode->Parent = parentNode;

    RedBlackTree_InsertRepair( redBlackTree, newNode );
    redBlackTree->Count++;
    return true;
}

bool RedBlackTree_NodeRemove( RedBlackTree* redBlackTree, RedBlackTreeNode* thisNode )
{
    if( !redBlackTree        ) return false;
    if( !redBlackTree->Root  ) return false;
    if( !redBlackTree->Count ) return false;
    if( !thisNode            ) return false;

    RedBlackTreeNode* parentNode  = thisNode->Parent;
    RedBlackTreeNode* currentNode = thisNode;

    RedBlackTreeNode* deletingNode;
    RedBlackTreeNode* replacingNode;
    bool currentIsRootNode = (currentNode == redBlackTree->Root);

    // Case 1: Leaf node:
    if( !currentNode->LeftChild && !currentNode->RightChild )
    {
        if ( currentIsRootNode ) redBlackTree->Root = NULL;
        else if ( parentNode->LeftChild == currentNode) parentNode->LeftChild  = NULL;
        else                                            parentNode->RightChild = NULL;

        replacingNode = NULL;
        deletingNode  = currentNode;
    }

    // Case 2: One Child
    else if( currentNode->LeftChild == NULL ^ currentNode->RightChild == NULL )
    {
        RedBlackTreeNode* childNode = currentNode->LeftChild ? currentNode->LeftChild : currentNode->RightChild;
        if ( currentIsRootNode ) redBlackTree->Root = childNode;
        else if ( parentNode->LeftChild == currentNode ) parentNode->LeftChild  = childNode;
        else                           parentNode->RightChild = childNode;

        replacingNode         = childNode;
        replacingNode->Parent = parentNode;
        deletingNode          = currentNode;
    }

    // Case 3: Both Children
    else
    {
        RedBlackTreeNode* inOrderParentNode = currentNode;
        RedBlackTreeNode* inOrderNode       = currentNode->LeftChild;
        bool inOrderIsLeftChild = true;
        while( inOrderNode->RightChild )
        {
            inOrderParentNode  = inOrderNode;
            inOrderNode        = inOrderNode->RightChild;
            inOrderIsLeftChild = false;
        }

        currentNode->Payload = inOrderNode->Payload;
        if ( inOrderIsLeftChild ) inOrderParentNode->LeftChild  = inOrderNode->LeftChild;
        else                      inOrderParentNode->RightChild = inOrderNode->LeftChild;
        replacingNode = inOrderNode->LeftChild;
        if( replacingNode ) replacingNode->Parent = inOrderParentNode;
        deletingNode = inOrderNode;
    }
    if( redBlackTree->Count != 1 ) RedBlackTree_RemoveRepair( redBlackTree, deletingNode, replacingNode );
    RedBlackTree_DeleteNode( redBlackTree, deletingNode );
    redBlackTree->Count--;  
    return true;
}

void RedBlackTree_PreOrderTraversal ( RedBlackTree* redBlackTree, void (*forEachNode)(RedBlackTree*, RedBlackTreeNode*) )
{
    if (!redBlackTree->Root) return;
    LinkedList* linkedList = LinkedList_NewList(NULL);
    LinkedList_AppendHead( linkedList, (LinkedListPayload) redBlackTree->Root );
    while ( linkedList->Count != 0 )
    {
        RedBlackTreeNode* currentNode = (RedBlackTreeNode*) linkedList->Head->Payload;
        if( forEachNode ) forEachNode( redBlackTree, currentNode );
        LinkedList_NodeRemove( linkedList, linkedList->Head );
        if( currentNode->RightChild ) LinkedList_AppendHead( linkedList, (LinkedListPayload) currentNode->RightChild );
        if( currentNode->LeftChild  ) LinkedList_AppendHead( linkedList, (LinkedListPayload) currentNode->LeftChild  );
    }
    LinkedList_DeleteList( linkedList );
}

void RedBlackTree_InOrderTraversal  ( RedBlackTree* redBlackTree, void (*forEachNode)(RedBlackTree*, RedBlackTreeNode*) )
{
    if ( !redBlackTree->Root ) return;
    LinkedList* linkedList = LinkedList_NewList(NULL);
    RedBlackTreeNode* currentNode = redBlackTree->Root;
    while (linkedList->Count != 0 || currentNode != NULL)
    {
        if (currentNode != NULL)
        {
            LinkedList_AppendHead( linkedList, (LinkedListPayload) currentNode );
            currentNode = currentNode->LeftChild;
        }
        else
        {
            currentNode = (RedBlackTreeNode*) linkedList->Head->Payload;
            RedBlackTreeNode* currentNode_rightChild = currentNode->RightChild;
            LinkedList_NodeRemove(linkedList, linkedList->Head);
            if( forEachNode ) forEachNode( redBlackTree, currentNode );
            currentNode = currentNode_rightChild;
        }
    }
    LinkedList_DeleteList( linkedList );
}

void RedBlackTree_PostOrderTraversal( RedBlackTree* redBlackTree, void (*forEachNode)(RedBlackTree*, RedBlackTreeNode*) )
{
    if( !redBlackTree->Root ) return;
    LinkedList* linkedList = LinkedList_NewList( NULL );
    RedBlackTreeNode* currentNode = redBlackTree->Root;
    LinkedList_AppendHead( linkedList, (LinkedListPayload) currentNode );
    while( linkedList->Count != 0 )
    {
        RedBlackTreeNode* nextNode = (RedBlackTreeNode*) linkedList->Head->Payload;
        bool finishedSubtrees = ( nextNode->RightChild == currentNode || nextNode->LeftChild == currentNode );
        bool isLeaf           = ( nextNode->RightChild == NULL        && nextNode->LeftChild == NULL        );
        if( finishedSubtrees || isLeaf )
        {
            LinkedList_NodeRemove( linkedList, linkedList->Head );
            forEachNode( redBlackTree, nextNode );
            currentNode = nextNode;
        }
        else
        {
            if( nextNode->RightChild )
                LinkedList_AppendHead( linkedList, (LinkedListPayload) nextNode->RightChild );
            if( nextNode->LeftChild  )
                LinkedList_AppendHead( linkedList, (LinkedListPayload) nextNode->LeftChild  );
        }
    }
    LinkedList_DeleteList( linkedList );
}

RedBlackTreeNode* RedBlackTree_NodeFind( RedBlackTree* redBlackTree, RedBlackTreePayload payload)
{
    RedBlackTreeNode* currentNode = redBlackTree->Root;
    while( currentNode != NULL )
    {
        char comparator = redBlackTree->LeftGreaterThanRight( payload, currentNode->Payload );
        if (comparator != true && comparator != false) return currentNode;
        currentNode = comparator ? currentNode->RightChild : currentNode->LeftChild;
    }
    return NULL;
}
#endif /* REDBLACKTREE_C */
