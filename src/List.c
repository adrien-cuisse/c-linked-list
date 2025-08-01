
#include <stdlib.h>

#include "../include/List.h"




/**
 * @brief - data shared by every node of a same list
 */
typedef struct list_header
{
	/**
	 * @brief - the first node of the list
	 */
	linked_list * first_node;

	/**
	 * @brief - the last node of the list
	 */
	linked_list * last_node;

	/**
	 * @brief - the whole size of the list, from first to last node
	 */
	size_t size;
} header;


struct linked_list
{
	/**
	 * @brief - the header shared by every node in the list
	 */
	header * header;

	/**
	 * @brief - the value stored in the node
	 */
	void * value;

	/**
	 * @brief - the previous node, NULL if none
	 */
	linked_list * previous;

	/**
	 * @brief - the next node, NULL if none
	 */
	linked_list * next;
};




/**
 * @brief - creates a blank header
 *
 * @return header * - the created header
 */
static header * create_blank_header(void)
{
	return calloc(1, sizeof(header));
}


/**
 * @brief - creates a node without bound header
 *
 * @param value - the value to store in the node
 *
 * @return linked_list * - the created node
 */
static linked_list * create_headerless_node(void * value)
{
	linked_list * list = calloc(1, sizeof(linked_list));
	if (list != NULL)
		list->value = value;

	return list;
}


/**
 * @brief - deletes the header and sets it to NULL
 *
 * @param header - the header to delete
 */
static void delete_header(header ** header)
{
	free(* header);
	* header = NULL;
}


/**
 * @brief - updates the header, setting last node and incrementing size
 *
 * @param node_to_append - the node that will be appended
 */
static void update_header_append(linked_list * node_to_append)
{
	header * header = node_to_append->header;

	if (header->first_node == NULL)
		header->first_node = node_to_append;

	header->last_node = node_to_append;
	header->size++;
}


/**
 * @brief - updates the header, setting first node and incrementing size
 *
 * @param node_to_append - the node that will be prepended
 */
static void update_header_prepend(linked_list * node_to_prepend)
{
	header * header = node_to_prepend->header;

	if (header->last_node == NULL)
		header->last_node = node_to_prepend;

	header->first_node = node_to_prepend;
	header->size++;
}


/**
 * @brief - updates the header, setting first/last nodes and decrementing size
 *
 * @param node_to_remove - the node that will be removed
 */
static void update_header_removal(linked_list * node_to_remove)
{
	header * header = node_to_remove->header;

	if (node_to_remove == header->first_node && node_to_remove->next != NULL)
		header->first_node = node_to_remove->next;
	if (node_to_remove == header->last_node && node_to_remove->previous != NULL)
		header->last_node = node_to_remove->previous;

	header->size--;
}


/**
 * @brief - creates a node to be bound with the given header and returns it,
 * 	the header to be bound will be updated
 *
 * @param value - the value to store in the node
 * @param header - the header to bind and update
 *
 * @return linked_list * - the created node
 */
static linked_list * create_node_and_update_header_append(
	void * value,
	header * header)
{
	linked_list * node = create_headerless_node(value);
	if (node == NULL)
		return NULL;

	node->header = header;

	update_header_append(node);

	return node;
}


/**
 * @brief - creates a node to be bound with the given header and returns it,
 * 	the header to be bound will be updated
 *
 * @param value - the value to store in the node
 * @param header - the header to bind and update
 *
 * @return linked_list * - the created node
 */
static linked_list * create_node_and_update_header_prepend(
	void * value,
	header * header)
{
	linked_list * node = create_headerless_node(value);
	if (node == NULL)
		return NULL;

	node->header = header;

	update_header_prepend(node);

	return node;
}


/**
 * @brief - deletes the current node and every previous ones,
 * 	doesn't check for NULL argument
 *
 * @param list - the list to delete, from the beginning to given node
 */
static void list_delete_backward(linked_list ** list)
{
	if (* list == NULL)
		return;

	list_delete_backward(& (* list)->previous);

	free(* list);
	* list = NULL;
}


/**
 * @brief - deletes the current node and every next ones,
 * 	doesn't check for NULL argument
 *
 * @param list - the list to delete, from given node to the end
 */
static void list_delete_forward(linked_list ** list)
{
	if (* list == NULL)
		return;

	list_delete_forward(& (* list)->next);

	free(* list);
	* list = NULL;
}


/**
 * @brief - link 2 nodes
 *
 * @param before - the previous node
 * @param after - the next node
 */
static void link_nodes(linked_list * before, linked_list * after)
{
	if (before != NULL)
		before->next = after;
	if (after != NULL)
		after->previous = before;
}




linked_list * list_create(void)
{
	return NULL;
}


void list_delete(linked_list ** list)
{
	if (list == NULL)
		return;

	if (* list == NULL)
		return;

	list_delete_backward(& (* list)->previous);
	list_delete_forward(& (* list)->next);

	free((* list)->header);
	(* list)->header = NULL;

	free(* list);
	* list = NULL;
}


size_t list_size(linked_list const * list)
{
	if (list == NULL)
		return 0;

	return list->header->size;
}


size_t list_size_forward(linked_list const * list)
{
	size_t size = 1;
	if (list == NULL)
		return 0;

	while ((list = list->next) != NULL)
		size++;

	return size;
}


size_t list_size_backward(linked_list const * list)
{
	size_t size = 1;
	if (list == NULL)
		return 0;

	while ((list = list->previous) != NULL)
		size++;

	return size;
}


void list_append(linked_list ** list, void * value)
{
	linked_list * old_tail;
	linked_list * new_tail;
	header * header;

	if (list == NULL)
		return;

	if (* list == NULL)
	{
		* list = create_node_and_update_header_append(
			value,
			create_blank_header());
		return;
	}

	header = (* list)->header;

	old_tail = header->last_node;
	new_tail = create_node_and_update_header_append(value, header);
	link_nodes(old_tail, new_tail);
}


void list_prepend(linked_list ** list, void * value)
{
	linked_list * old_head;
	linked_list * new_head;
	header * header;

	if (list == NULL)
		return;

	if (* list == NULL)
	{
		* list = create_node_and_update_header_prepend(
			value,
			create_blank_header());
		return;
	}

	header = (* list)->header;

	old_head = header->first_node;
	new_head = create_node_and_update_header_prepend(value, header);
	link_nodes(old_head, new_head);
}


void list_remove_node(linked_list ** list)
{
	linked_list * node_to_remove;
	header * header;

	if (list == NULL)
		return;

	node_to_remove = * list;
	header = (* list)->header;

	link_nodes(node_to_remove->previous, node_to_remove->next);
	update_header_removal(node_to_remove);

	if (header->size == 0) /* last node removed = orphan header */
		delete_header(& header);

	* list = node_to_remove->next;
	free(node_to_remove);
}


linked_list * list_next(linked_list const * list)
{
	if (list == NULL)
		return NULL;

	return list->next;
}


linked_list * list_previous(linked_list const * list)
{
	if (list == NULL)
		return NULL;

	return list->previous;
}


linked_list * list_head(linked_list const * list)
{
	if (list == NULL)
		return NULL;

	return list->header->first_node;
}


linked_list * list_tail(linked_list const * list)
{
	if (list == NULL)
		return NULL;

	return list->header->last_node;
}


void * list_content(linked_list const * list)
{
	if (list == NULL)
		return NULL;

	return list->value;
}


void * list_reduce(
	linked_list const * list,
	void * accumulator,
	void (* reducer)(void * accumulator, void const * node_content))
{
	while (list != NULL)
	{
		reducer(accumulator, list->value);
		list = list->next;
	}

	return accumulator;
}
