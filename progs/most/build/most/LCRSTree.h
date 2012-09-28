#ifndef LCRS_TREE_H
#define LCRS_TREE_H

#include <stdio.h>
#include <stdlib.h>

typedef struct tagLCRSNode 
{
    struct tagLCRSNode* LeftChild;
    struct tagLCRSNode* RightSibling;

    void *Data;
} LCRSNode;

typedef struct test_struct
{
	unsigned char test;
}test;

LCRSNode* LCRS_CreateNode( void *NewData );
void      LCRS_DestroyNode( LCRSNode* Node );
void      LCRS_DestroyTree( LCRSNode* Root );

void      LCRS_AddChildNode( LCRSNode* ParentNode, LCRSNode *ChildNode);
void      LCRS_PrintTree( LCRSNode* Node, int Depth );

#endif LCRS_TREE_H