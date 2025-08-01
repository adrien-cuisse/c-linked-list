
#ifndef LIST_HEADER
#define LIST_HEADER

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>




/**
 * @brief - a doubly linked list
 */
typedef struct linked_list linked_list;




/**
 * @brief - creates an empty list and returns it
 * 	Complexity: O(1)
 *
 * @return linked_list * - the created list, NULL if allocation failed
 */
linked_list * list_create(void);


/**
 * @brief - deletes the list and sets the current node to NULL
 * 	Complexity: O(n)
 *
 * @param list - the list to delete
 */
void list_delete(linked_list ** list);


/**
 * @brief - measures the size of the list, from its first node to its last node
 * 	Complexity: O(1)
 *
 * @param list - the list to measure
 *
 * @return size_t - the size of the list, from its first node to its last node
 */
size_t list_size(linked_list const * list);


/**
 * @brief - measures the size of the list, from given node to the end
 * 	Complexity: O(n)
 *
 * @param list - the node to measure from
 *
 * @return size_t - the size of the list, from the given node to the end
 */
size_t list_size_forward(linked_list const * list);


/**
 * @brief - measures the size of the list, from its start node to the given node
 * 	Complexity: O(n)
 *
 * @param list - the node to measure to
 *
 * @return size_t - the size of the list, from its start to the given node
 */
size_t list_size_backward(linked_list const * list);


/**
 * @brief - adds a node at the end of the list
 * 	Complexity: O(1)
 *
 * @param list - the list to append a node to
 * @param value - the value to store in the the list
 */
void list_append(linked_list ** list, void * value);


/**
 * @brief - adds a node at the beginning of the list
 * 	Complexity: O(1)
 *
 * @param list - the list to prepend a node to
 * @param value - the value to store in the the list
 */
void list_prepend(linked_list ** list, void * value);


/**
 * @brief - removes the given node from the list
 * 	Complexity: O(1)
 *
 * @param node - the node to remove
 */
void list_remove_node(linked_list ** node);


/**
 * @brief - returns the previous node
 * 	Complexity: O(1)
 *
 * @param list - the node to get the previous one from
 *
 * @return linked_list * - the previous node, NULL if none
 */
linked_list * list_previous(linked_list const * list);


/**
 * @brief - returns the next node
 * 	Complexity: O(1)
 *
 * @param list - the node to get the next one from
 *
 * @return linked_list * - the next node, NULL if none
 */
linked_list * list_next(linked_list const * list);


/**
 * @brief - returns the first node of the list
 * 	Complexity: O(1)
 *
 * @param list - the list to get the first node from
 *
 * @return linked_list * - the first node of the list, NULL if none
 */
linked_list * list_head(linked_list const * list);


/**
 * @brief - returns the last node of the list
 * 	Complexity: O(1)
 *
 * @param list - the list to get the last node from
 *
 * @return linked_list * - the last node of the list, NULL if none
 */
linked_list * list_tail(linked_list const * list);


/**
 * @brief - returns the value stored in the node
 * 	Complexity: O(1)
 *
 * @param list - the node to get the value from
 *
 * @return void const * - the value stored in the node, NUll if none
 */
void * list_content(linked_list const * list);


/**
 * @brief - collects every value stored in the list
 * 	Complexity: O(n)
 *
 * @param list - the list to iterate
 * @param accumulator - the initial value of the accumulator
 * @param callback - the callback to apply on every node
 *
 * @return - the accumulator
 */
void * list_reduce(
	linked_list const * list,
	void * accumulator,
	void (* reducer)(void * accumulator, void const * node_content));




#ifdef __cplusplus
}
#endif

#endif
