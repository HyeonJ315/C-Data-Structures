#ifndef TD_LINKEDLIST_C
#define TD_LINKEDLIST_C

#include "../LinkedList.h"
#include <stdio.h>

void TD_NodeDeleter( LinkedListPayload payload )
{
    //free(payload);
}

void TD_LinkedListTestDriver()
{
    printf_s( "Creating a new linked list...  \n" );
    LinkedList* linkedList = LinkedList_NewList( TD_NodeDeleter );
    LinkedList_DebugPrint( linkedList );

    printf( "Inserting the first node with value 0 as payload.\n" );
    LinkedList_AppendHead( linkedList, 0 );
    LinkedList_DebugPrint( linkedList );

    printf("Removing the head node.\n");
    LinkedList_NodeRemove( linkedList, linkedList->Head );
    LinkedList_DebugPrint( linkedList );

    printf( "Inserting five nodes.\n " );
    LinkedList_AppendTail( linkedList, 0 );
    LinkedList_AppendHead( linkedList, 1 );
    LinkedList_AppendTail( linkedList, 2 );
    LinkedList_AppendHead( linkedList, 3 );
    LinkedList_AppendTail( linkedList, 4 );
    LinkedList_DebugPrint( linkedList );
    
    printf( "Removing 2nd element from list. \n ");
    LinkedList_NodeRemove( linkedList, linkedList->Head->Next );
    LinkedList_DebugPrint( linkedList );

    printf( "Adding 5 after 2nd element from list. \n " );
    LinkedList_InsertAfter( linkedList, linkedList->Head->Next, 5 );
    LinkedList_DebugPrint(linkedList);

    printf( "Adding 6 before 3nd element from list. \n " );
    LinkedList_InsertBefore( linkedList, linkedList->Head->Next->Next, 6 );
    LinkedList_DebugPrint(linkedList);

    printf( "Removing head from list. \n " );
    LinkedList_NodeRemove(linkedList, linkedList->Head );
    LinkedList_DebugPrint(linkedList);

    printf( "Removing tail from list. \n " );
    LinkedList_NodeRemove(linkedList, linkedList->Tail );
    LinkedList_DebugPrint(linkedList);

    printf( "Clearing linked list. \n");
    LinkedList_ClearList( linkedList );
    LinkedList_DebugPrint(linkedList);

    printf( "Adding 7 to the list. \n" );
    LinkedList_AppendTail( linkedList, 7 );
    LinkedList_DebugPrint(linkedList);

    printf( "Adding 8 after head. \n ");
    LinkedList_InsertAfter( linkedList, linkedList->Head, 8 );
    LinkedList_DebugPrint(linkedList);

    printf( "Removing head. \n" )    ;
    LinkedList_NodeRemove( linkedList, linkedList->Head );
    LinkedList_DebugPrint(linkedList);

    printf("Adding 9 before head. \n ");
    LinkedList_InsertBefore(linkedList, linkedList->Head, 9);
    LinkedList_DebugPrint(linkedList);

    printf( "Removing tail. \n ");
    LinkedList_NodeRemove( linkedList, linkedList->Tail );
    LinkedList_DebugPrint( linkedList );

    printf("Adding 10 after tail. \n ");
    LinkedList_InsertAfter(linkedList, linkedList->Tail, 10);
    LinkedList_DebugPrint(linkedList);

    printf( "Destroying list. \n" );
    LinkedList_DeleteList( linkedList );
    return;
}

#endif