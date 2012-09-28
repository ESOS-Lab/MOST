#ifndef QUEUE_H
#define QUEUE_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

typedef struct tagNode
{
	unsigned long num;
	struct timeval time;
	struct tagNode* NextNode;
}Node;

typedef struct tagLinkedQueue
{	
	Node* Front;
	Node* Rear;
	int Count;
}LinkedQueue;

void CreateQueue(LinkedQueue** Queue);
void DestroyQueue(LinkedQueue* Queue);

Node* CreateNode(void);
void DestroyNode(Node* _Node);

void Enqueue(LinkedQueue* Queue, Node* NewNode);
Node* Dequeue(LinkedQueue* Queue);

int IsEmpty(LinkedQueue* Queue);
#endif
