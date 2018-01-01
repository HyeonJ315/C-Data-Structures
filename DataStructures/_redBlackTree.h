#ifndef _REDBLACKTREE_H
#define _REDBLACKTREE_H

#include <stdbool.h>

typedef enum _redBlackTreeColor
{
    Red   = 0,
    Black = 1,
    DoubleBlack = 2,
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
    int Depth;
    int Count;
    bool ( *LeftGreaterThanRight ) ( RedBlackTreePayload left, RedBlackTreePayload right );
    void ( *PayloadDeleter       ) ( RedBlackTreePayload payload                         );
    RedBlackTreeNode* Root;
} RedBlackTree;

#endif