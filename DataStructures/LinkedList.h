#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include "_linkedList.h"
#include <stdbool.h>

LinkedListNode* LinkedList_NewNode   ( LinkedListPayload payload                              );
LinkedList*     LinkedList_NewList   ( void(*PayloadDeleter)(LinkedListPayload)               );
void            LinkedList_DeleteNode( LinkedList* linkedList, LinkedListNode* linkedListNode );
void            LinkedList_DeleteList( LinkedList* linkedList                                 );
void            LinkedList_ClearList ( LinkedList* linkedList                                 );

bool LinkedList_InsertAfter    ( LinkedList* linkedList, LinkedListNode* thisNode, LinkedListPayload payload );
bool LinkedList_InsertBefore   ( LinkedList* linkedList, LinkedListNode* thisNode, LinkedListPayload payload );
bool LinkedList_RemoveNode     ( LinkedList* linkedList, LinkedListNode* thisNode  );
bool LinkedList_AppendTailNode ( LinkedList* linkedList, LinkedListPayload payload );
bool LinkedList_AppendHeadNode ( LinkedList* linkedList, LinkedListPayload payload );
LinkedListNode* LinkedList_Find( LinkedList* linkedList, LinkedListPayload payload );

void LinkedList_DebugPrint(LinkedList* linkedList);
#endif