#include <stdio.h>
#ifndef REDBLACKTREE_C
#define REDBLACKTREE_C

#include "RedBlackTree.h"
#include "LinkedList.h"
#include <stdlib.h>
#include <stdbool.h>

RedBlackTree* RedBlackTree_NewTree( bool (*comparator)( RedBlackTreePayload,RedBlackTreePayload ), 
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
    int depth = 0;
    while (currentNode != NULL)
    {
        parentNode = currentNode;
        comparatorCurr = redBlackTree->LeftGreaterThanRight( newNode->Payload, currentNode->Payload );
        currentNode = comparatorCurr ? currentNode->RightChild : currentNode->LeftChild;
        depth++;
    }
    if (parentNode)
    {
        if (comparatorCurr) parentNode->RightChild = newNode;
        else                parentNode->LeftChild  = newNode;
    }
    newNode->Parent = parentNode;

    RedBlackTree_InsertRepair(redBlackTree, newNode, depth);
    redBlackTree->Count++;
    return true;
}

bool RedBlackTree_NodeRemove( RedBlackTree* redBlackTree, RedBlackTreeNode* thisNode )
{
    if( !redBlackTree ) return false;
    if( !thisNode     ) return false;

    RedBlackTreeNode* parentNode  = thisNode->Parent;
    RedBlackTreeNode* currentNode = thisNode;


    RedBlackTreeNode* deletingNode;
    RedBlackTreeNode* replacingNode;

    bool currentIsLeftChild = false;
    if ( parentNode ) currentIsLeftChild = ( parentNode->LeftChild == currentNode );

    // Case 1: Leaf node:
    if( !currentNode->LeftChild && !currentNode->RightChild )
    {
        if( currentNode == redBlackTree->Root ) redBlackTree->Root = NULL;
        else if ( currentIsLeftChild  ) parentNode->LeftChild  = NULL;
        else                            parentNode->RightChild = NULL;
        deletingNode  = currentNode;
        replacingNode = NULL;
    }

    // Case 2: One Child
    else if( currentNode->LeftChild == NULL ^ currentNode->RightChild == NULL )
    {
        RedBlackTreeNode* childNode = currentNode->LeftChild ? currentNode->LeftChild : currentNode->RightChild;
        if( currentNode == redBlackTree->Root ) redBlackTree->Root = childNode;
        else if ( currentIsLeftChild  ) parentNode->LeftChild  = NULL;
        else                            parentNode->RightChild = NULL;
        deletingNode  = currentNode;
        replacingNode = childNode;
    }

    // Case 3: Both Children
    else
    {
        RedBlackTreeNode* parentReplacementNode = currentNode;
        RedBlackTreeNode* replacementNode       = currentNode->LeftChild;

        bool replacementIsLeftChild = true;
        while( replacementNode->RightChild )
        {
            parentReplacementNode  = replacementNode;
            replacementNode        = replacementNode->RightChild;
            replacementIsLeftChild = false;
        }

        currentNode->Payload = replacementNode->Payload;
        if ( replacementIsLeftChild ) currentNode->LeftChild            = replacementNode->LeftChild;
        else                          parentReplacementNode->RightChild = replacementNode->LeftChild;

        deletingNode  = replacementNode;
        replacingNode = replacementNode->LeftChild;
    }

    RedBlackTree_RemoveRepair( redBlackTree, deletingNode, replacingNode );
    RedBlackTree_DeleteNode( redBlackTree, deletingNode );
    redBlackTree->Count--;  
    return true;
}

void RedBlackTree_RemoveRepair( RedBlackTree* redBlackTree, RedBlackTreeNode* deletingNode, RedBlackTreeNode* replacingNode )
{
    if( !redBlackTree ) return;
    if( !deletingNode ) return;

    RedBlackTreeColor deletingNodeColor  = deletingNode  ? deletingNode->Color  : Black;
    RedBlackTreeColor replacingNodeColor = replacingNode ? replacingNode->Color : Black;
    
    RedBlackTreeNode* parentNode = deletingNode->Parent;

    // Case 1: Either deletingNode or replacingNode is Red.
    if (deletingNodeColor == Red ^ replacingNodeColor == Red)
        if (replacingNode) replacingNode->Color = Black;

    // Case 2: Both deletingNode and replacingNode are black.4
    //TODO: Finish code for repairing after BST removal of a node.
    /*
    else if (deletingNodeColor == Black && replacingNodeColor == Black)
    {
        replacingNodeColor = DoubleBlack;
        while (replacingNodeColor == DoubleBlack || redBlackTree->Root != replacingNode )
        {
            if (replacingNode == redBlackTree->Root)
            {
                replacingNode->Color = Black;
            }
        }
    }*/
}

void RedBlackTree_InsertRepair( RedBlackTree* redBlackTree, RedBlackTreeNode* currentNode, int depth )
{
    if( !currentNode ) return;
    
    // Case 1: Current node is the root.
    if ( !currentNode->Parent )
    {
        currentNode->Color = Black; 
        redBlackTree->Root = currentNode;
        return;
    }

    RedBlackTreeNode *grandParentNode = currentNode->Parent->Parent;
    RedBlackTreeNode *parentNode      = currentNode->Parent;
    RedBlackTreeNode *uncleNode       = NULL;

    if( grandParentNode &&  grandParentNode->RightChild == parentNode )
        uncleNode = grandParentNode->LeftChild;
    if( grandParentNode &&  grandParentNode->LeftChild  == parentNode )
        uncleNode = grandParentNode->RightChild;

    RedBlackTreeColor uncleColor  = uncleNode ? uncleNode->Color : Black;
    RedBlackTreeColor parentColor = parentNode->Color;

    // Case 2: Parent is black.
    if( parentColor == Black ) return;

    // Case 3: parent and uncle are red.
    if( parentColor == Red && uncleColor == Red )
    {
        RedBlackTree_NodeRecolor( grandParentNode );
        RedBlackTree_NodeRecolor( parentNode      );
        RedBlackTree_NodeRecolor( uncleNode       );
        redBlackTree->Depth = max( redBlackTree->Depth, depth );
        RedBlackTree_InsertRepair( redBlackTree, grandParentNode, depth );
        return;
    }

    // Case 4: Parent is red, but uncle is black.
    if( parentColor == Red && uncleColor == Black )
    {
        // Part A1: Left Triangle
        if( grandParentNode && grandParentNode->LeftChild && 
            grandParentNode->LeftChild == parentNode && 
            parentNode->RightChild     == currentNode )
        {
            RedBlackTree_NodeShiftLeft ( redBlackTree, parentNode      );
            RedBlackTree_NodeShiftRight( redBlackTree, grandParentNode );
            RedBlackTree_NodeRecolor( currentNode     );
            RedBlackTree_NodeRecolor( grandParentNode );
            redBlackTree->Depth = max( redBlackTree->Depth, --depth );
            return;
        }
        // Part A2: Right Triangle
        if( grandParentNode && grandParentNode->RightChild && 
            grandParentNode->RightChild == parentNode &&
            parentNode->LeftChild == currentNode )
        {
            RedBlackTree_NodeShiftRight( redBlackTree, parentNode      );
            RedBlackTree_NodeShiftLeft ( redBlackTree, grandParentNode );
            RedBlackTree_NodeRecolor( currentNode     );
            RedBlackTree_NodeRecolor( grandParentNode );
            redBlackTree->Depth = max( redBlackTree->Depth, --depth );
            return;
        }

        // Part B1: Left Line
        if( grandParentNode && grandParentNode->LeftChild && 
            grandParentNode->LeftChild == parentNode && 
            parentNode->LeftChild == currentNode )
        {
            RedBlackTree_NodeShiftRight( redBlackTree, grandParentNode );
            RedBlackTree_NodeRecolor( parentNode      );
            RedBlackTree_NodeRecolor( grandParentNode );
            redBlackTree->Depth = max( redBlackTree->Depth, depth );
            return;
        }

        // Part B2: Right Line
        if( grandParentNode && grandParentNode->RightChild && 
            grandParentNode->RightChild == parentNode && 
            parentNode->RightChild == currentNode )
        {
            RedBlackTree_NodeShiftLeft( redBlackTree, grandParentNode );
            RedBlackTree_NodeRecolor( parentNode      );
            RedBlackTree_NodeRecolor( grandParentNode );
            redBlackTree->Depth = max( redBlackTree->Depth, depth );
            return;
        }
    }
}

bool RedBlackTree_NodeRecolor( RedBlackTreeNode* redBlackTreeNode )
{
    if( !redBlackTreeNode ) return false;
    if (redBlackTreeNode->Color == Red) redBlackTreeNode->Color = Black;
    else                                redBlackTreeNode->Color = Red;
    return true;
}

bool RedBlackTree_NodeShiftLeft ( RedBlackTree* redBlackTree, RedBlackTreeNode* redBlackTreeNode )
{
    if ( !redBlackTreeNode                        ) return false;
    if ( !redBlackTreeNode->RightChild            ) return false;

    RedBlackTreeNode* greatGrandParentNode = redBlackTreeNode->Parent;
    RedBlackTreeNode* grandParentNode      = redBlackTreeNode;
    RedBlackTreeNode* parentNode           = redBlackTreeNode->RightChild;
    RedBlackTreeNode* currentNode          = redBlackTreeNode->RightChild->LeftChild;

    if ( greatGrandParentNode )
    {
        if ( greatGrandParentNode->RightChild == grandParentNode )
            greatGrandParentNode->RightChild = parentNode;
        else
            greatGrandParentNode->LeftChild = parentNode;
    }

    parentNode->Parent          = grandParentNode->Parent;
    parentNode->LeftChild       = grandParentNode;
    grandParentNode->Parent     = parentNode;
    grandParentNode->RightChild = currentNode;
    if( currentNode ) currentNode->Parent = grandParentNode;
    if( grandParentNode == redBlackTree->Root ) redBlackTree->Root = parentNode;
    return true;
}

bool RedBlackTree_NodeShiftRight( RedBlackTree* redBlackTree, RedBlackTreeNode* redBlackTreeNode )
{
    if ( !redBlackTreeNode                        ) return false;
    if ( !redBlackTreeNode->LeftChild             ) return false;

    RedBlackTreeNode* greatGrandParentNode = redBlackTreeNode->Parent;
    RedBlackTreeNode* grandParentNode      = redBlackTreeNode;
    RedBlackTreeNode* parentNode           = redBlackTreeNode->LeftChild;
    RedBlackTreeNode* currentNode          = redBlackTreeNode->LeftChild->RightChild;

    if ( greatGrandParentNode )
    {
        if ( greatGrandParentNode->RightChild == grandParentNode ) 
            greatGrandParentNode->RightChild = parentNode;
        else
            greatGrandParentNode->LeftChild = parentNode;
    }

    parentNode->Parent          = greatGrandParentNode;
    parentNode->RightChild      = grandParentNode;
    grandParentNode->Parent     = parentNode;
    grandParentNode->LeftChild  = currentNode;
    if( currentNode ) currentNode->Parent = grandParentNode;
    if( grandParentNode == redBlackTree->Root ) redBlackTree->Root = parentNode;
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

#endif /* REDBLACKTREE_C */