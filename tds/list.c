#include "list.h"
#include <minix/syslib.h>


List* List_Create(){
	List* l = (List *)malloc(sizeof(List));

	ListNode * node = (ListNode*)malloc(sizeof(ListNode));
	node->nextNode=NULL;

	l->node = node;
	return l;
}

int List_Count(List* list){
	ListNode* node = list->node;
	int n=-1;
	while(node!=NULL){
		node= node->nextNode;
		n++;
	}
	return n;
}

char List_Empty(List* list){
	ListNode* node = list->node;
	if(node->nextNode == NULL)
		return 1;
	return 0;
}

void List_Add(List* list, void* conteudo){
	ListNode* node = list->node;
	while(node->nextNode!=NULL){
		node= node->nextNode;
	}
	node->nextNode = (ListNode*)malloc(sizeof(ListNode));
	node->nextNode->nextNode = NULL;

	node->nextNode->conteudo = conteudo;
}

void List_AddFront(List* list, void* conteudo){
	ListNode* node = list->node->nextNode;
	list->node->nextNode = (ListNode*)malloc(sizeof(ListNode));

	list->node->nextNode->conteudo = conteudo;
	list->node->nextNode->nextNode = node;
}

void List_Remove(List* list, void* conteudo){
	ListNode* node = list->node;
	while(node->nextNode->conteudo!=conteudo){
		node= node->nextNode;
		if(node->nextNode == NULL)
			return;
	}
	void* newNext = node->nextNode->nextNode;
	free(node->nextNode);

	node->nextNode = newNext;
}

int List_Contain(List* list, void * conteudo){
	ListNode* node = list->node;
	while(node!=NULL){
		if(node->conteudo == conteudo)
			return 1;
		node= node->nextNode;

	}
	return 0;
}

void List_Destroy(List* l){
	ListNode* node = l->node;

	while(node!=NULL){
		ListNode *dest = node;
		node = dest->nextNode;
		free(dest);
	}

	free(l);
	l=NULL;
}
