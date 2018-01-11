#ifndef LINKEDLIST_C
#define LINKEDLIST_C

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "_linkedList.h"
#include "LinkedList.h"

LinkedListNode* LinkedList_NewNode( LinkedListPayload payload )
{
    LinkedListNode* linkedListNode = malloc( sizeof(LinkedList) );
    if ( !linkedListNode ) return NULL;
    linkedListNode->Next = linkedListNode;
    linkedListNode->Prev = linkedListNode;
    linkedListNode->Payload = payload;
    return linkedListNode;
}

LinkedList* LinkedList_NewList( void (*PayloadDeleter)(LinkedListPayload) )
{
    LinkedList* linkedList = calloc( 1, sizeof(LinkedList) );
    if( !linkedList ) return NULL;
    linkedList->PayloadDeleter = PayloadDeleter;
    return linkedList;
}

void LinkedList_DeleteNode( LinkedList* linkedList, LinkedListNode* linkedListNode )
{
    if (linkedList->PayloadDeleter )
    {
        linkedList->PayloadDeleter( linkedListNode->Payload );
    }
    free( linkedListNode );
}

void LinkedList_DeleteList( LinkedList* linkedList )
{
    LinkedListNode* curr = linkedList->Head;
    while( linkedList->Count > 0 )
    {
        LinkedListNode* tmp = curr->Next;
        LinkedList_DeleteNode( linkedList, curr );
        curr = tmp;
        linkedList->Count--;
    }
    free( linkedList );
}

void LinkedList_ClearList( LinkedList* linkedList )
{
    LinkedListNode* curr = linkedList->Head;
    while( linkedList->Count > 0 )
    {
        LinkedListNode* tmp = curr->Next;
        LinkedList_DeleteNode(linkedList, curr);
        curr = tmp;
        linkedList->Count--;
    }
    linkedList->Head  = NULL;
    linkedList->Tail  = NULL;
}

bool LinkedList_InsertAfter( LinkedList* linkedList, LinkedListNode* thisNode, LinkedListPayload payload )
{
    if ( !linkedList ) return false;
    if ( !thisNode   ) return false;
    LinkedListNode* newNode = LinkedList_NewNode( payload );
    if ( !LinkedList_NodeInsertAfter(linkedList, thisNode, newNode) )
    {
        LinkedList_DeleteNode(linkedList, newNode);
        return false;
    }
    return true;
}

bool LinkedList_InsertBefore( LinkedList* linkedList, LinkedListNode* thisNode, LinkedListPayload payload )
{
    if ( !linkedList ) return false;
    if ( !thisNode   ) return false;
    LinkedListNode* newNode = LinkedList_NewNode( payload );
    if (!LinkedList_NodeInsertBefore(linkedList, thisNode, newNode))
    {
        LinkedList_DeleteNode(linkedList, newNode);
        return false;
    }
    return true;
}

bool LinkedList_AppendTail( LinkedList* linkedList, LinkedListPayload payload )
{
    if (!linkedList) return false;
    
    if (linkedList->Count > 0)
    {
        return LinkedList_InsertAfter(linkedList, linkedList->Tail, payload);
    }
    linkedList->Count++;
    return linkedList->Head = linkedList->Tail = LinkedList_NewNode( payload );
}

bool LinkedList_AppendHead(LinkedList* linkedList, LinkedListPayload payload )
{
    if (!linkedList) return false;

    if (linkedList->Count > 0)
    {
        return LinkedList_InsertBefore(linkedList, linkedList->Head, payload);
    }
    linkedList->Count++;
    return linkedList->Head = linkedList->Tail = LinkedList_NewNode( payload );
}

bool LinkedList_NodeInsertAfter(LinkedList * linkedList, LinkedListNode * thisNode, LinkedListNode * newNode)
{
    if ( !linkedList ) return false;
    if ( !thisNode   ) return false;
    if ( !newNode    ) return false;

    LinkedListNode* thisNode_Next = thisNode->Next;
    thisNode->Next = newNode;
    newNode->Prev = thisNode;
    newNode->Next = thisNode_Next;
    thisNode_Next->Prev = newNode;
    if (linkedList->Tail == thisNode)
    {
        linkedList->Tail = newNode;
        linkedList->Head->Prev = newNode;
    }
    linkedList->Count++;
    return true;
}

bool LinkedList_NodeInsertBefore(LinkedList* linkedList, LinkedListNode* thisNode, LinkedListNode* newNode)
{
    if ( !linkedList ) return false;
    if ( !thisNode   ) return false;
    if ( !newNode    ) return false;

    LinkedListNode* thisNode_Prev = thisNode->Prev;
    thisNode->Prev = newNode;
    newNode->Next = thisNode;
    newNode->Prev = thisNode_Prev;
    thisNode_Prev->Next = newNode;
    if (linkedList->Head == thisNode)
    {
        linkedList->Head       = newNode;
        linkedList->Tail->Next = newNode;
    }
    linkedList->Count++;
    return true;
}

bool LinkedList_NodeAppendTail(LinkedList * linkedList, LinkedListNode * newNode)
{
    if (!linkedList) return false;
    if (!newNode   ) return false;

    if (linkedList->Count > 0)
    {
        return LinkedList_NodeInsertAfter(linkedList, linkedList->Tail, newNode );
    }
    linkedList->Count++;
    return NULL != (linkedList->Head = linkedList->Tail = newNode);
}

bool LinkedList_NodeAppendHead(LinkedList * linkedList, LinkedListNode * newNode)
{
    if (!linkedList) return false;
    if (!newNode   ) return false;

    if (linkedList->Count > 0)
    {
        return LinkedList_NodeInsertBefore(linkedList, linkedList->Head, newNode );
    }
    linkedList->Count++;
    return NULL != (linkedList->Head = linkedList->Tail = newNode);
}

bool LinkedList_NodeRemove( LinkedList* linkedList, LinkedListNode* thisNode )
{
    if ( !linkedList ) return false;
    if ( !thisNode   ) return false;
    LinkedListNode* leftNode  = thisNode->Prev;
    LinkedListNode* rightNode = thisNode->Next;

    LinkedList_DeleteNode( linkedList, thisNode );
    
    switch( linkedList->Count )
    {
        case 1:
        {
            linkedList->Head = NULL;
            linkedList->Tail = NULL;
            break;
        }
        case 2:
        {
            LinkedListNode* singleNode = leftNode = rightNode;
            singleNode->Next = singleNode;
            singleNode->Prev = singleNode;
            linkedList->Head = singleNode;
            linkedList->Tail = singleNode;
            break;
        }
        default:
        {
            leftNode->Next  = rightNode;
            rightNode->Prev = leftNode;
            if( thisNode == linkedList->Head ) linkedList->Head = rightNode;
            if( thisNode == linkedList->Tail ) linkedList->Tail = leftNode;
            break;
        }
    }
    linkedList->Count--;
    return true;
}

LinkedListNode* LinkedList_Find( LinkedList* linkedList, LinkedListPayload payload )
{
    LinkedListNode* head = linkedList->Head;
    LinkedListNode* curr = linkedList->Head;
    do
    {
        if (payload == curr->Payload) return curr;
        curr = head->Next;
    }
    while (curr != head);
    return NULL;
}

void LinkedList_DebugPrint(LinkedList* linkedList)
{
    LinkedListNode* head = linkedList->Head;
    LinkedListNode* curr = linkedList->Head;
    printf( "Printing state of linked list: \n Count: %d\n Head: 0x%p\n Tail: 0x%p\n", 
                linkedList->Count, 
                linkedList->Head, 
                linkedList->Tail );
    if( linkedList->Count == 0 ) return;
    do
    {
        printf("   Prev Node : 0x%p\n", curr->Prev);
        printf("   Node      : 0x%p, PayloadArr: 0x%p \n", curr, curr->Payload);
        printf("   Next Node : 0x%p\n", curr->Next);
        curr = curr->Next;
    } while (head != curr);
}

#endif