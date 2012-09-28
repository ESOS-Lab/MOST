#ifndef DOUBLY_LINKEDLIST_H
#define DOUBLY_LINKEDLIST_H

#include <stdio.h>
#include <stdlib.h>

typedef struct tagNode
{
    void *Data;
    struct tagNode* PrevNode;
    struct tagNode* NextNode;
} LinkNode;

/* 함수 원형 선언 */
LinkNode* DLL_CreateNode( void * NewData );
void  DLL_DestroyNode( LinkNode* Node);
void  DLL_AppendNode( LinkNode** Head, LinkNode* NewNode );
void  DLL_InsertAfter( LinkNode* Current, LinkNode* NewNode );
void  DLL_RemoveNode( LinkNode** Head, LinkNode* Remove );
LinkNode* DLL_GetNodeAt( LinkNode* Head, int Location );
int   DLL_GetNodeCount( LinkNode* Head );
void DLL_DestroyList( LinkNode* Head);

#endif DOUBLY_LINKEDLIST_H
