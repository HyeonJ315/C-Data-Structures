#ifndef REDBLACKTREE_H
#define REDBLACKTREE_H
#include "_redBlackTree.h"
#include <stdbool.h>

RedBlackTree* RedBlackTree_NewTree( bool (*comparator)(RedBlackTreePayload,RedBlackTreePayload), 
                                    void (*payloadDeleter)( RedBlackTreePayload ) );
RedBlackTreeNode* RedBlackTree_NewNode( RedBlackTreePayload payload );

void RedBlackTree_DeleteTree( RedBlackTree* redBlackTree );
void RedBlackTree_DeleteNode( RedBlackTree* redBlackTree, RedBlackTreeNode* redBlackTreeNode );
bool RedBlackTree_InsertNode( RedBlackTree* redBlackTree, RedBlackTreePayload payload        );

void RedBlackTree_PreOrderTraversal ( RedBlackTree* redBlackTree, void (*forEachNode)(RedBlackTree*, RedBlackTreeNode*) );
void RedBlackTree_InOrderTraversal  ( RedBlackTree* redBlackTree, void (*forEachNode)(RedBlackTree*, RedBlackTreeNode*) );
void RedBlackTree_PostOrderTraversal( RedBlackTree* redBlackTree, void (*forEachNode)(RedBlackTree*, RedBlackTreeNode*) );

bool RedBlackTree_RecolorNode        ( RedBlackTreeNode* redBlackTreeNode );
bool RedBlackTree_SingleLeftRotation ( RedBlackTreeNode* parentNode       );
bool RedBlackTree_SingleRightRotation( RedBlackTreeNode* parentNode       );
bool RedBlackTree_DoubleLeftRotation ( RedBlackTreeNode* grandParentNode  );
bool RedBlackTree_DoubleRightRotation( RedBlackTreeNode* grandParentNode  );

#endif /* REDBLACKTREE_H */
