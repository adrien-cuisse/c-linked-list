
#include "../../include/List.h"

#include "utils.h"




static linked_list * list_of_size(size_t size)
{
	linked_list * list = list_create();

	list_append(& list, "head");
	list_append(& list, "second node");
	list_append(& list, "third node");

	for (size_t current_size = 3; current_size < size - 1; current_size++)
		list_append(& list, "padding");

	list_append(& list, "tail");

	return list;
}


linked_list * small_list(void)
{
	return list_of_size(4);
}


linked_list * big_list(void)
{
	return list_of_size(BIG_LIST_SIZE);
}
