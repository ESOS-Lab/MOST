#include "LCRSTree.h"
#include "fat.h"

LCRSNode* LCRS_CreateNode( void *NewData )
{
    LCRSNode* NewNode = (LCRSNode*)malloc( sizeof(LCRSNode) );
    NewNode->LeftChild    = NULL;
    NewNode->RightSibling = NULL;
    NewNode->Data = NewData;

    return NewNode;
}

void LCRS_DestroyNode( LCRSNode* Node )
{
	DirInfo *dir=NULL;

	dir = (DirInfo *)Node->Data;

	//if(Node->Data != NULL)
	if(dir!=NULL)
	{
		DLL_DestroyList(dir->head);
		free(Node->Data);
	}

	if(Node != NULL)
		free(Node);
}

void LCRS_DestroyTree( LCRSNode* Root )
{
    if ( Root->RightSibling != NULL )
        LCRS_DestroyTree( Root->RightSibling );

    if ( Root->LeftChild != NULL )
        LCRS_DestroyTree( Root->LeftChild );

    Root->LeftChild = NULL;
    Root->RightSibling = NULL;

    LCRS_DestroyNode( Root );
}

void LCRS_AddChildNode( LCRSNode* Parent, LCRSNode *Child)
{
    if ( Parent->LeftChild == NULL )
    {
        Parent->LeftChild = Child;
    }
    else 
    {
        LCRSNode* TempNode = Parent->LeftChild;
        while ( TempNode->RightSibling != NULL )
            TempNode = TempNode->RightSibling;

        TempNode->RightSibling = Child;        
    }
}
