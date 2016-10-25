#ifndef _LIST_H
#define _LIST_H

/** @defgroup List List module
 * @{
 *
 * Module responsible for Lists
 *
 */

/** @name ListNode structure */
/** @{
 *
 * ListNode structure
 */
typedef struct ListNode{

	void* conteudo;

	struct ListNode* nextNode;

}ListNode;
/** @} end of ListNode */

/** @name List structure */
/** @{
 *
 * List structure
 */
typedef struct{
	ListNode* node;
}List;
/** @} end of List */

/**
 * @brief Function to create a List
 *
 *This function creates a list, it creates the first node of the list (the header)
 *
 *
 * @return Returns a pointer to the new list created
 */
List* List_Create();

/**
 * @brief Function to add a new element
 *
 *This function adds a new element to an existing list
 *
 * @param list list in which the new node will be added
 * @param conteudo the contents of the lists new node
 *
 * @return void
 */
void List_Add(List* list, void* conteudo);

/**
 * @brief Function to add a new element
 *
 *This function adds a new element to the front of an existing list
 *
 * @param list list in which the new node will be added
 * @param conteudo the contents of the lists new node
 *
 * @return void
 */
void List_AddFront(List* list, void* conteudo);

/**
 * @brief Function to remove an element
 *
 *This function removes an element from a given list
 *
 * @param list list in which the element will be removed
 * @param conteudo contents of the node to be deleted
 *
 * @return void
 */
void List_Remove(List* list, void* conteudo);

/**
 * @brief Function to check if a list is empty
 *
 *This function verifies if the list is empty
 *
 * @param list list to be checked

 * @return char 1 if true or 0 if false
 */
char List_Empty(List* list);

/**
 * @brief Function to check if a list has an element
 *
 *This function verifies if the list contains the specified element
 *
 * @param list list to be checked
 * @param conteudo contents of the node we are looking for

 * @return char 1 if true or 0 if false
 */
int List_Contain(List* list, void * conteudo);

/**
 * @brief Function to see the size of a list
 *
 *This function counts the number of elements on a given list it does not count the front of the list (the header)
 *
 * @param list list to be checked
 *
 * @return int the number of elements of the list
 */
int List_Count(List* list);

/**
 * @brief Function to delete a list
 *
 *This function delete a list and frees it's memory
 *
 * @param list list to be deleted
 *
 * @return void
 */
void List_Destroy(List* l);

#endif
