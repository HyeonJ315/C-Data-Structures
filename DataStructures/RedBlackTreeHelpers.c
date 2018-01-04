#ifndef REDBLACKTREEHELPERS_C
#define REDBLACKTREEHELPERS_C

#include "RedBlackTreeHelpers.h"
#include "_redBlackTree.h"
#include <stdlib.h>
#include <stdio.h>
void RedBlackTree_RemoveRepair( RedBlackTree* redBlackTree, RedBlackTreeNode* deletingNode,  RedBlackTreeNode* replacingNode )
{
    if( !redBlackTree         ) return;
    if( !deletingNode         ) return;
    if( !deletingNode->Parent )
    {
        if (replacingNode) replacingNode->Color = Black;
        return;
    }

    RedBlackTreeNode* parentNode = deletingNode->Parent;
    bool replacementIsLeftChild = true;
    if( (parentNode->LeftChild || parentNode->RightChild) && 
         parentNode->RightChild == replacingNode        ) replacementIsLeftChild = false;
    RedBlackTreeNode* siblingNode = replacementIsLeftChild ? parentNode->RightChild : parentNode->LeftChild;

    RedBlackTreeColor replacingNodeColor = replacingNode ? replacingNode->Color : Black;
    RedBlackTreeColor deletingNodeColor  = deletingNode  ? deletingNode->Color  : Black;

    // Case 1. There is a red node.
    if( replacingNodeColor == Red || deletingNodeColor == Red )
    {
        if ( replacingNode ) replacingNode->Color = Black;
        return;
    }

    // Case 2. Both are black nodes.
    else
    {
        typedef struct _internalNode InternalNode;
        typedef struct _internalNode
        {
            RedBlackTreeColor Color;
            InternalNode* Parent;
            InternalNode* LeftChild;
            InternalNode* RightChild;
            RedBlackTreeNode* Reference;
        } InternalNode;

        InternalNode currParentNode;
        InternalNode currNode;
        InternalNode currSiblingNode;
        InternalNode currLeftChildNode;
        InternalNode currRightChildNode;

        currParentNode.Reference      = deletingNode->Parent;
        currParentNode.Color          = deletingNode->Parent ? deletingNode->Parent->Color : Black;
        currParentNode.Parent         = NULL;
        currParentNode.LeftChild      = replacementIsLeftChild ? &currNode : &currSiblingNode;
        currParentNode.RightChild     = replacementIsLeftChild ? &currSiblingNode : &currNode;
        
        currNode.Reference            = replacingNode;
        currNode.Color                = DoubleBlack;
        currNode.Parent               = &currParentNode;
        currNode.LeftChild            = NULL;
        currNode.RightChild           = NULL;

        currSiblingNode.Reference     = siblingNode;
        currSiblingNode.Color         = siblingNode ? siblingNode->Color : Black;
        currSiblingNode.Parent        = &currParentNode;
        currSiblingNode.LeftChild     = &currLeftChildNode;
        currSiblingNode.RightChild    = &currRightChildNode;

        currLeftChildNode.Reference   = siblingNode ? siblingNode->LeftChild : NULL;
        currLeftChildNode.Color       = siblingNode && siblingNode->LeftChild ? siblingNode->LeftChild->Color : Black;
        currLeftChildNode.Parent      = &currSiblingNode;
        currLeftChildNode.LeftChild   = NULL;
        currLeftChildNode.RightChild  = NULL;

        currRightChildNode.Reference  = siblingNode ? siblingNode->RightChild : NULL;
        currRightChildNode.Color      = siblingNode && siblingNode->RightChild ? siblingNode->RightChild->Color : Black;
        currRightChildNode.Parent     = &currSiblingNode;
        currRightChildNode.LeftChild  = NULL;
        currRightChildNode.RightChild = NULL;
        int Counter = 0;
        while( currNode.Color == DoubleBlack && (RedBlackTreeNode*) currNode.Reference != redBlackTree->Root )
        {
            printf( "Counter!: %d\n", Counter++ );
            // Current sibling is red.
            if (currSiblingNode.Color == Red)
            {
                if( currParentNode.LeftChild == &currSiblingNode )
                {
                    RedBlackTree_NodeShiftRight( redBlackTree,  currParentNode.Reference );

                    if( currParentNode.Reference  ) currParentNode.Reference->Color  = Red;
                    if( currSiblingNode.Reference ) currSiblingNode.Reference->Color = Black;

                    // Try again with a new tree.
                    currParentNode.Color = Red;
                    currNode.Color       = DoubleBlack;

                    currSiblingNode.Reference    = currParentNode.Reference     ? currParentNode.Reference->LeftChild   : NULL;
                    currSiblingNode.Color        = currSiblingNode.Reference    ? currSiblingNode.Reference->Color      : Black;
                    currLeftChildNode.Reference  = currSiblingNode.Reference    ? currSiblingNode.Reference->LeftChild  : NULL;
                    currLeftChildNode.Color      = currLeftChildNode.Reference  ? currLeftChildNode.Reference->Color    : Black;
                    currRightChildNode.Reference = currSiblingNode.Reference    ? currSiblingNode.Reference->RightChild : NULL;
                    currRightChildNode.Color     = currRightChildNode.Reference ? currRightChildNode.Reference->Color   : Black;
                }

                else
                {
                    RedBlackTree_NodeShiftLeft( redBlackTree,  currParentNode.Reference );

                    if( currParentNode.Reference  ) currParentNode.Reference->Color  = Red;
                    if( currSiblingNode.Reference ) currSiblingNode.Reference->Color = Black;

                    // Try again with a new tree.
                    currParentNode.Color = Red;
                    currNode.Color       = DoubleBlack;

                    currSiblingNode.Reference    = currParentNode.Reference     ? currParentNode.Reference->RightChild  : NULL;
                    currSiblingNode.Color        = currSiblingNode.Reference    ? currSiblingNode.Reference->Color      : Black;
                    currLeftChildNode.Reference  = currSiblingNode.Reference    ? currSiblingNode.Reference->LeftChild  : NULL;
                    currLeftChildNode.Color      = currLeftChildNode.Reference  ? currLeftChildNode.Reference->Color    : Black;
                    currRightChildNode.Reference = currSiblingNode.Reference    ? currSiblingNode.Reference->RightChild : NULL;
                    currRightChildNode.Color     = currRightChildNode.Reference ? currRightChildNode.Reference->Color   : Black;
                }
            }

            // Children are red.
            else if( currLeftChildNode.Color == Red || currRightChildNode.Color == Red )
            {

                // Left Left Case
                if ( currParentNode.LeftChild == &currSiblingNode && currLeftChildNode.Color == Red )
                {
                    RedBlackTree_NodeShiftRight( redBlackTree,  currParentNode.Reference );
                    if ( currSiblingNode.Reference   ) currSiblingNode.Reference->Color   = currParentNode.Reference->Color;
                    if ( currParentNode.Reference    ) currParentNode.Reference->Color    = Black;
                    if ( currLeftChildNode.Reference ) currLeftChildNode.Reference->Color = Black;
                }

                // Left Right Case
                else if ( currParentNode.LeftChild == &currSiblingNode && currRightChildNode.Color == Red )
                {
                    RedBlackTree_NodeShiftLeft ( redBlackTree,  currSiblingNode.Reference );
                    RedBlackTree_NodeShiftRight( redBlackTree,  currParentNode.Reference  );
                    if ( currRightChildNode.Reference ) currRightChildNode.Reference->Color = currParentNode.Reference->Color;
                    if ( currParentNode.Reference     ) currParentNode.Reference->Color     = Black;
                    if ( currSiblingNode.Reference    ) currSiblingNode.Reference->Color    = Black;
                }
                
                // Right Right Case 
                else if ( currParentNode.RightChild == &currSiblingNode && currRightChildNode.Color == Red )
                {
                    RedBlackTree_NodeShiftLeft( redBlackTree,  currParentNode.Reference );
                    if ( currSiblingNode.Reference    ) currSiblingNode.Reference->Color    = currParentNode.Reference->Color;
                    if ( currParentNode.Reference     ) currParentNode.Reference->Color     = Black;
                    if ( currRightChildNode.Reference ) currRightChildNode.Reference->Color = Black;
                }
                
                // Right Left Case
                else if ( currParentNode.RightChild == &currSiblingNode && currLeftChildNode.Color == Red )
                {
                    RedBlackTree_NodeShiftRight( redBlackTree,  currSiblingNode.Reference );
                    RedBlackTree_NodeShiftLeft ( redBlackTree,  currParentNode.Reference  );
                    if ( currLeftChildNode.Reference ) currLeftChildNode.Reference->Color = currParentNode.Reference->Color;
                    if ( currParentNode.Reference    ) currParentNode.Reference->Color    = Black;
                    if ( currSiblingNode.Reference   ) currSiblingNode.Reference->Color   = Black;
                }
                return;
            }

            // Children are black
            else
            {
                // Parent is red.
                if( currParentNode.Color == Red )
                {
                    if( currSiblingNode.Reference ) currSiblingNode.Reference->Color = Red;
                    if( currParentNode.Reference  ) currParentNode.Reference->Color  = Black;
                    if( currNode.Reference        ) currNode.Reference->Color        = Black;
                    return;
                }

                // Parent is black. Try again with a new tree.

                if (currNode.Reference) currNode.Reference->Color = Black;

                currParentNode.Reference = currParentNode.Reference ? currParentNode.Reference->Parent : NULL;
                currParentNode.Color     = currParentNode.Reference ? currParentNode.Reference->Color  : Black;
                currNode.Reference       = currNode.Reference ? currNode.Reference->Parent : NULL;
                currNode.Color           = currNode.Reference ? currNode.Reference->Color  : Black;
                if ( currParentNode.Reference && currParentNode.Reference->LeftChild == currNode.Reference )
                {
                    currParentNode.LeftChild  = &currNode;
                    currParentNode.RightChild = &currSiblingNode;
                    currSiblingNode.Reference = currParentNode.Reference  ? currParentNode.Reference->RightChild : NULL;
                    currSiblingNode.Color     = currSiblingNode.Reference ? currSiblingNode.Reference->Color     : Black;
                }
                else
                {
                    currParentNode.LeftChild  = &currSiblingNode;
                    currParentNode.RightChild = &currNode;
                    currSiblingNode.Reference = currParentNode.Reference  ? currParentNode.Reference->LeftChild : NULL;
                    currSiblingNode.Color     = currSiblingNode.Reference ? currSiblingNode.Reference->Color    : Black;
                }
                currLeftChildNode.Reference  = currSiblingNode.Reference    ? currSiblingNode.Reference->LeftChild  : NULL;
                currLeftChildNode.Color      = currLeftChildNode.Reference  ? currLeftChildNode.Reference->Color    : Black;
                currRightChildNode.Reference = currSiblingNode.Reference    ? currSiblingNode.Reference->RightChild : NULL;
                currRightChildNode.Color     = currRightChildNode.Reference ? currRightChildNode.Reference->Color   : Black;
            }
        }
    }
}

void RedBlackTree_InsertRepair( RedBlackTree* redBlackTree, RedBlackTreeNode* currentNode )
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
        RedBlackTree_InsertRepair( redBlackTree, grandParentNode );
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

bool RedBlackTree_Assert( RedBlackTree* redBlackTree )
{
    if( !redBlackTree )                    return false;
    if( redBlackTree->Count == 0 && redBlackTree->Root != NULL ) return false;
    if( !redBlackTree->Root )              return true;
    if( redBlackTree->Root->Color == Red ) return false;


    RedBlackTreeNode* currentNode = redBlackTree->Root;
    return true;

}
#endif