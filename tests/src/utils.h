
#ifndef LIST_UTILS_HEADER
#define LIST_UTILS_HEADER

#define BIG_LIST_SIZE 10000000

#include "../../include/List.h"



/**
 * @brief - creates a list with a few elements (less than 10) and returns it
 *
 * @return linked_list * - the created list
 */
linked_list * small_list(void);


/**
 * @brief - creates a list with a lot of elements (expected to be > 1 million)
 * 	and returns
 *
 * @return linked_list * - the linked list
 */
linked_list * big_list(void);




#endif /* LIST_UTILS_HEADER */
