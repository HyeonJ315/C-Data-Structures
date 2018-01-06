#ifndef TD_REDBLACKTREE_H
#define TD_REDBLACKTREE_H

#include "../RedBlackTree.h"e

bool TD_RedBlackTreeComparator( RedBlackTreePayload leftNode, RedBlackTreePayload rightNode );
void TD_RedBlackTreePayloadDeleter( RedBlackTreePayload payload );
void TD_RedBlackTreeTestDriver();

#endif