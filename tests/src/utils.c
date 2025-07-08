
#include "../../include/List.h"

#include "utils.h"




static linked_list * list_of_size(size_t size)
{
	linked_list * list = list_create();

	list_add(& list, "head");
	list_add(& list, "second node");
	list_add(& list, "third node");

	for (size_t current_size = 3; current_size < size - 1; current_size++)
		list_add(& list, "padding");

	list_add(& list, "tail");

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
