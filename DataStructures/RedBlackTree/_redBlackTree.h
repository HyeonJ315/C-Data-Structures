#ifndef _REDBLACKTREE_H
#define _REDBLACKTREE_H

typedef enum _redBlackTreeColor
{
    Red   = 0,
    Black = 1,
} RedBlackTreeColor;

typedef int* RedBlackTreePayload;

typedef struct _redBlackTreeNode RedBlackTreeNode;

typedef struct _redBlackTreeNode
{
    RedBlackTreeColor Color;
    RedBlackTreeNode* Parent;
    RedBlackTreeNode* LeftChild;
    RedBlackTreeNode* RightChild;
    RedBlackTreePayload Payload;
} RedBlackTreeNode;

typedef struct _redBlackTree
{
    int Count;
    // Return -1 if left is equal to right!!!!
    char ( *LeftGreaterThanRight ) ( RedBlackTreePayload left, RedBlackTreePayload right );
    void ( *PayloadDeleter       ) ( RedBlackTreePayload payload                         );
    RedBlackTreeNode* Root;
} RedBlackTree;

#endif