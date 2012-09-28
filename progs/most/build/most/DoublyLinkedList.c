#include "DoublyLinkedList.h"
#include "fat.h"

/*  노드 생성 */
LinkNode* DLL_CreateNode( void *NewData )
{
    LinkNode* NewNode = (LinkNode*)malloc(sizeof(LinkNode));

    NewNode->Data = NewData;
    NewNode->PrevNode = NULL;
    NewNode->NextNode = NULL;

    return NewNode;
}

/*  노드 소멸 */
void DLL_DestroyNode( LinkNode* Node )
{
	if(Node->Data != NULL)
	{
		free(Node->Data);
	}

    free(Node);
}

/*  노드 추가 */
void DLL_AppendNode( LinkNode** Head, LinkNode* NewNode )
{
    /*  헤드 노드가 NULL이라면 새로운 노드가 Head */
    if ( (*Head) == NULL ) 
    {
        *Head = NewNode;
    } 
    else
    {
        /*  테일을 찾아 NewNode를 연결한다. */
        LinkNode* Tail = (*Head);
        while ( Tail->NextNode != NULL )
        {
            Tail = Tail->NextNode;
        }

        Tail->NextNode = NewNode;
        NewNode->PrevNode = Tail; /*  기존의 테일을 새로운 테일의 PrevNode가 가리킨다. */
    }
}

/*  노드 삽입 */
void DLL_InsertAfter( LinkNode* Current, LinkNode* NewNode )
{
    NewNode->NextNode = Current->NextNode;
    NewNode->PrevNode = Current;

    if ( Current->NextNode != NULL )
    {
        Current->NextNode->PrevNode = NewNode;
        Current->NextNode = NewNode;
    }
}

/*  노드 제거 */
void DLL_RemoveNode( LinkNode** Head, LinkNode* Remove )
{
    if ( *Head == Remove )
    {
        *Head = Remove->NextNode;
        if ( (*Head) != NULL )
            (*Head)->PrevNode = NULL;
        
        Remove->PrevNode = NULL;
        Remove->NextNode = NULL;
    }
    else
    {
        LinkNode* Temp = Remove;

        if ( Remove->PrevNode != NULL )
            Remove->PrevNode->NextNode = Temp->NextNode;

        if ( Remove->NextNode != NULL )
            Remove->NextNode->PrevNode = Temp->PrevNode;

        Remove->PrevNode = NULL;
        Remove->NextNode = NULL;
    }    
}

/*  노드 탐색 */
LinkNode* DLL_GetNodeAt( LinkNode* Head, int Location )
{
    LinkNode* Current = Head;

    while ( Current != NULL && (--Location) >= 0)
    {
        Current = Current->NextNode;
    }

    return Current;
}

/*  노드 수 세기 */
int DLL_GetNodeCount( LinkNode* Head )
{
    unsigned int  Count = 0;
    LinkNode*         Current = Head;

    while ( Current != NULL )
    {
        Current = Current->NextNode;
        Count++;
    }

    return Count;
}

void DLL_DestroyList( LinkNode* Head)
{
    int   i;
    int   count   = 0;
    LinkNode* current = NULL;
	FileInfo *file=NULL;
	extern RBTNode* Nil;

    count = DLL_GetNodeCount(Head);

    for (i=0; i<count; i++ )
    {
        current = DLL_GetNodeAt( Head, 0 );

        if ( current != NULL ) 
        {
        	//Destory RB tree
			file = (FileInfo *)current->Data;
			if(file!=NULL)
			{
				if(file->tree != NULL)
				{
					Nil=file->nil;
					RBT_DestroyTree( file->tree );
				}
				
				if(file->nil != NULL)
					free(file->nil);
			}

            DLL_RemoveNode( &Head, current );
            DLL_DestroyNode( current );
        }
    }
}

void PrintNode( LinkNode* _Node )
{
#if 0
    if ( _Node->PrevNode == NULL )
        printf("Prev: NULL");
    else
        printf("Prev: %d", _Node->PrevNode->Data);

    printf(" Current: %d ", _Node->Data);

    if ( _Node->NextNode == NULL )
        printf("Next: NULL\n");
    else
        printf("Next: %d\n", _Node->NextNode->Data);
#endif
}
