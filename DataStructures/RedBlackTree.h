#ifndef REDBLACKTREE_H
#define REDBLACKTREE_H

#include "_redBlackTree.h"
#include <stdbool.h>

RedBlackTree* RedBlackTree_NewTree( bool (*comparator)(RedBlackTreePayload,RedBlackTreePayload), 
                                    void (*payloadDeleter)( RedBlackTreePayload ) );
RedBlackTreeNode* RedBlackTree_NewNode( RedBlackTreePayload payload );

void RedBlackTree_DeleteTree( RedBlackTree* redBlackTree );
void RedBlackTree_DeleteNode( RedBlackTree* redBlackTree, RedBlackTreeNode* redBlackTreeNode );

bool RedBlackTree_Insert    ( RedBlackTree* redBlackTree, RedBlackTreePayload payload );
bool RedBlackTree_NodeInsert( RedBlackTree* redBlackTree, RedBlackTreeNode* newNode   );
bool RedBlackTree_NodeRemove( RedBlackTree* redBlackTree, RedBlackTreeNode* thisNode  );

void RedBlackTree_PreOrderTraversal ( RedBlackTree* redBlackTree, void (*forEachNode)(RedBlackTree*, RedBlackTreeNode*) );
void RedBlackTree_InOrderTraversal  ( RedBlackTree* redBlackTree, void (*forEachNode)(RedBlackTree*, RedBlackTreeNode*) );
void RedBlackTree_PostOrderTraversal( RedBlackTree* redBlackTree, void (*forEachNode)(RedBlackTree*, RedBlackTreeNode*) );

void RedBlackTree_RemoveRepair  ( RedBlackTree* redBlackTree, RedBlackTreeNode* deletingNode, RedBlackTreeNode* replacingNode );
void RedBlackTree_InsertRepair  ( RedBlackTree* redBlackTree, RedBlackTreeNode* redBlackTreeNode, int depth );

bool RedBlackTree_NodeRecolor   ( RedBlackTreeNode* redBlackTreeNode );
bool RedBlackTree_NodeShiftLeft ( RedBlackTree* redBlackTree, RedBlackTreeNode* grandParentNode  );
bool RedBlackTree_NodeShiftRight( RedBlackTree* redBlackTree, RedBlackTreeNode* grandParentNode  );

#endif /* REDBLACKTREE_H */
