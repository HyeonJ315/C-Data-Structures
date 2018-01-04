#ifndef _LINKEDLIST_H
#define _LINKEDLIST_H

typedef struct _linkedListNode LinkedListNode;

typedef int* LinkedListPayload;

// ( Circular linked list )
typedef struct _linkedListNode
{
    LinkedListNode* Prev;
    LinkedListNode* Next;
    LinkedListPayload Payload;
} LinkedListNode;

typedef struct _linkedList
{
    int Count;
    LinkedListNode* Head;
    LinkedListNode* Tail;
    void(*PayloadDeleter)(LinkedListPayload);
} LinkedList;

#endif