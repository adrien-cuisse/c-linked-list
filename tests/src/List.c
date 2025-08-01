
#include <criterion/criterion.h>
#include <signal.h>
#include <time.h>

#include "../../include/List.h"

#include "utils.h"

/**
 * Some operations are in constant time O(1), but involve big lists to test
 * Those tests are disabled by default, since there's no reason they would go
 * back to O(n) complexity
 */
// #define DO_CONSTANT_TIME_BENCHMARK_TESTS




Test(linked_list, is_empty_on_creation)
{
	// given a new list
	linked_list * list = list_create();

	// when checking its size
	size_t size = list_size(list);

	// then it should be 0
	cr_assert_eq(size, 0, "list is not empty");
}


Test(linked_list, append_to_null_has_no_effect)
{
	// given no list
	linked_list ** to_nowhere = NULL;

	// when trying to append anything
	list_append(to_nowhere, "any value");

	// then it shouldn't crash
}


Test(linked_list, is_not_empty_after_append)
{
	// given an empty list
	linked_list * list = list_create();

	// when appending a value
	list_append(& list, "any value");

	// then it shouldn't be empty anymore
	cr_assert_neq(list_size(list), 0, "list is still empty");
}


Test(linked_list, append_increase_lenght_by_1)
{
	// given a list with a few elements
	linked_list * list = small_list();
	size_t previous_size = list_size(list);

	// when appending another one
	list_append(& list, "foo");

	// then the list should have its length increased
	size_t new_size = list_size(list);
	cr_assert_eq(previous_size + 1, new_size, "length wasn't incremented");
}


Test(linked_list, append_adds_at_the_end)
{
	// given a list with a few elements
	linked_list * list = small_list();
	linked_list * old_tail = list_tail(list);

	// when appending another one
	list_append(& list, "foo");

	// then the last element should be the new one
	linked_list * new_tail = list_tail(list);
	cr_assert_neq(old_tail, new_tail, "element wasn't added at the end");
}


Test(linked_list, next_gives_next_node)
{
	// given a list
	linked_list * list = small_list();
	size_t whole_size = list_size_forward(list);

	// when moving to the next node
	linked_list * iterator = list_next(list);

	// then the list from that node should be lesser by 1
	size_t size_from_position = list_size_forward(iterator);
	cr_assert_eq(
		whole_size - 1,
		size_from_position,
		"forward size from next node should be lesser by 1");
}


Test(linked_list, next_of_null_doesnt_crash)
{
	// given no list
	linked_list * from_no_list = NULL;

	// when trying moving to the next node
	list_next(from_no_list);

	// then it souldn't crash
}


Test(linked_list, previous_gives_previous_node)
{
	// given a list with a few elements
	linked_list * list_beginning = small_list();

	// when moving to the next node then back
	linked_list * iterator = list_previous(list_next(list_beginning));

	// then we should be back to the beginning
	cr_assert_eq(
		list_beginning,
		iterator,
		"should be back to the beginning of the list");
}


Test(linked_list, previous_of_null_doesnt_crash)
{
	// given no list
	linked_list * from_no_list = NULL;

	// when trying moving to the previous node
	list_previous(from_no_list);

	// then it souldn't crash
}


Test(linked_list, remove_from_list_of_length_1_makes_it_empty)
{
	// given a list of 1 element
	linked_list * list = list_create();
	list_append(& list, "1");

	// when removing it
	list_remove_node(& list);

	// then the list should have a length of 0
	size_t size = list_size(list);
	cr_assert_eq(0, size, "list is not empty");
}


Test(linked_list, remove_from_beginning_of_list_decrease_length_by_1)
{
	// given a list with a few elements
	linked_list * list = small_list();
	size_t previous_size = list_size(list);

	// when removing the first element
	list_remove_node(& list);

	// then the list should have its length decreased by 1
	size_t new_size = list_size(list);
	cr_assert_eq(previous_size - 1, new_size, "length wasn't decremented");
}


Test(linked_list, remove_from_middle_of_list_decrease_length_by_1)
{
	// given a list with a few elements
	linked_list * list = small_list();
	size_t previous_size = list_size(list);

	// when removing the element in the middle
	linked_list * middle_node = list_next(list);
	list_remove_node(& middle_node);

	// then the list should have its length decreased by 1
	size_t new_size = list_size(list);
	cr_assert_eq(previous_size - 1, new_size, "length wasn't decremented");
}


Test(linked_list, remove_from_end_of_list_decrease_length_by_1)
{
	// given a list with a few elements
	linked_list * list = small_list();
	size_t previous_size = list_size(list);

	// when removing the element at the end
	linked_list * last_node = list;
	while (list_next(last_node) != NULL)
		last_node = list_next(last_node);
	list_remove_node(& last_node);

	// then the list should have its length decreased by 1
	size_t new_size = list_size(list);
	cr_assert_eq(previous_size - 1, new_size, "length wasn't decremented");
}


Test(linked_list, remove_from_null_has_no_effect)
{
	// given no list
	linked_list ** from_nowhere = NULL;

	// when trying ao removing a node
	list_remove_node(from_nowhere);

	// then it shouldn't crash
}


Test(linked_list, values_are_accessed_in_appending_order)
{
	// given a list of 2 elements
	linked_list * list = list_create();
	list_append(& list, "first");
	list_append(& list, "second");

	// when accessing the value of the current node
	void const * actual_value = list_content(list);

	// then it should be the first value appended
	cr_assert_eq(
		actual_value,
		"first",
		"returned value isn't the first appended");
}


static void store_values_in_buffer_reducer(
	void * accumulator,
	void const * value)
{
	static int index = 0;

	((char *) accumulator)[index++] = ((char *) value)[0];
}


Test(linked_list, reduce_applies_to_every_node_in_appending_order)
{
	// given a list of 2 elements
	linked_list * list = list_create();
	list_append(& list, "1");
	list_append(& list, "2");

	// when applying a callback to its nodes
	char callback_buffer[3] = { 0 };
	char * reduced = list_reduce(
		list,
		callback_buffer,
		store_values_in_buffer_reducer);

	// then it should have accessed each node in appending order
	cr_assert_str_eq(
		"12",
		reduced,
		"not all nodes have been visited in appending order");
}


Test(linked_list, delete_deletes_previous_nodes, .signal = SIGSEGV)
{
	// given a list with a few elements
	linked_list * list = small_list();

	// when deleting the list from a middle node
	linked_list * second_node = list_next(list);
	list_delete(& second_node);

	// then the first node shouldn't exist anymore (SEGFAULT)
	cr_assert_str_eq(
		"first element",
		list_content(list),
		"previous nodes are still accessible");
}


Test(linked_list, delete_deletes_next_nodes, .signal = SIGSEGV)
{
	// given a list with a few elements
	linked_list * list = small_list();
	linked_list * second_node = list_next(list);

	// when deleting the list from any node
	list_delete(& list);

	// then next nodes shouldn't exist anymore (SEGFAULT)
	cr_assert_str_eq(
		"second node",
		list_content(second_node),
		"next nodes are still accessible");
}


Test(linked_list, delete_deletes_current_node)
{
	// given a list with a few elements
	linked_list * list = small_list();

	// when deleting the list from any node
	list_delete(& list);

	// then the node should have been set to NULL
	cr_assert_null(list, "node hasn't been freed and set to NULL");
}


Test(linked_list, delete_from_null_doesnt_crash)
{
	// given no list
	linked_list ** from_nowhere = NULL;

	// when trying to delete from it
	list_delete(from_nowhere);

	// then it shouldn't crash
}


Test(linked_list, deleting_empty_doesnt_crash)
{
	// given an empty list
	linked_list * empty_list = list_create();

	// when deleting it
	list_delete(& empty_list);

	// then it shouldn't crash
}


Test(linked_list, list_content_on_null_doesnt_crash)
{
	// given no list
	linked_list * of_nothing = NULL;

	// when trying to extract the stored value
	list_content(of_nothing);

	// then it shouldn't crash
}


Test(linked_list, sizing_backwards_gives_distance_to_start_of_list)
{
	// given a list with a few elements
	linked_list * list = small_list();

	// when measuring the list backwards from the 3rd node
	linked_list * third_node = list_next(list_next(list));
	size_t size_to_start = list_size_backward(third_node);

	// then it should be 3
	cr_assert_eq(
		size_to_start,
		3,
		"distance from start to third node should be 3");
}


Test(linked_list, sizing_backwards_on_null_doesnt_crash)
{
	// given no list
	linked_list * of_nothing = NULL;

	// when trying to measure its backwards size
	list_size_backward(of_nothing);

	// then it shouldn't crash
}


Test(linked_list, sizing_forwards_on_null_doesnt_crash)
{
	// given no list
	linked_list * of_nothing = NULL;

	// when trying to measure its backwards size
	list_size_forward(of_nothing);

	// then it shouldn't crash
}


Test(linked_list, sizing_null_list_doesnt_crash)
{
	// given no list
	linked_list * of_nothing = NULL;

	// when trying to measure it
	list_size(of_nothing);

	// then it shouldn't crash
}


Test(linked_list, head_returns_first_element)
{
	// given a list with a few elements
	linked_list * list = small_list();

	// when querying the head from any node
	linked_list * third_node = list_next(list_next(list));
	linked_list * head = list_head(third_node);

	// then it should be the first element
	cr_assert_eq(head, list, "head should be the first element");
}


Test(linked_list, head_on_null_doesn_crash)
{
	// given no list
	linked_list * of_nothing = NULL;

	// when querying the head from it
	list_head(of_nothing);

	// then it shouldn't crash
}


Test(linked_list, tail_returns_last_element)
{
	// given a list with a few elements
	linked_list * list = small_list();

	linked_list * last_node = list;
	while (list_next(last_node) != NULL)
		last_node = list_next(last_node);

	// when querying the tail from any node
	linked_list * tail = list_tail(list);

	// then it should be the last element
	cr_assert_eq(tail, last_node, "tail should be the last element");
}


Test(linked_list, tail_on_null_doesn_crash)
{
	// given no list
	linked_list * of_nothing = NULL;

	// when querying the tail from it
	list_tail(of_nothing);

	// then it shouldn't crash
}


Test(linked_list, prepend_to_null)
{
	// given no list
	linked_list ** to_nowhere = NULL;

	// when trying to prepend anything
	list_prepend(to_nowhere, "any value");

	// then it shouldn't crash
}


Test(linked_list, is_not_empty_after_prepend)
{
	// given an empty list
	linked_list * list = list_create();

	// when prepending a value
	list_prepend(& list, "any value");

	// then it shouldn't be empty anymore
	cr_assert_neq(list_size(list), 0, "list is still empty");
}


Test(linked_list, prepend_increase_lenght_by_1)
{
	// given a list with a few elements
	linked_list * list = small_list();
	size_t previous_size = list_size(list);

	// when prepending another one
	list_prepend(& list, "foo");

	// then the list should have its length increased
	size_t new_size = list_size(list);
	cr_assert_eq(previous_size + 1, new_size, "length wasn't incremented");
}


Test(linked_list, prepend_adds_at_the_beginning)
{
	// given a list with a few elements
	linked_list * list = small_list();
	linked_list * old_head = list_head(list);

	// when prepending another one
	list_prepend(& list, "foo");

	// then the first element should be the new one
	linked_list * new_head = list_head(list);
	cr_assert_neq(old_head, new_head, "element wasn't added at the beginning");
}




#ifdef DO_CONSTANT_TIME_BENCHMARK_TESTS

static double benchmark_tailing_time(linked_list * list)
{
	clock_t start = clock();
	list_tail(list);
	clock_t end = clock();
	return ((double)(end - start)) / CLOCKS_PER_SEC;
}


Test(linked_list, tail_is_in_constant_time)
{
	// given 2 list, a small one and a big one
	linked_list * small = small_list();
	linked_list * big = big_list();

	// when measuring time it takes to get the last element on the small list
	double small_list_tailing_time = benchmark_tailing_time(small);
	// ... and measuring time it takes to get the last element on the big list
	double big_list_tailing_time = benchmark_tailing_time(big);

	// then both time should be close
	double epsilon = small_list_tailing_time;
	cr_assert_float_eq(
		small_list_tailing_time,
		big_list_tailing_time,
		epsilon,
		"getting to the last node isn't in constant time");
}


static double benchmark_heading_time(linked_list * list)
{
	clock_t start = clock();
	list_head(list);
	clock_t end = clock();
	return ((double)(end - start)) / CLOCKS_PER_SEC;
}


Test(linked_list, head_is_in_constant_time)
{
	// given a big list
	linked_list * huge_list = big_list();

	// when measuring time it takes to get the first element from an early node
	linked_list * early_node = list_next(huge_list);
	double early_node_heading_time = benchmark_heading_time(early_node);
	// ... and measuring time it takes to get the first node from the last one
	linked_list * last_node = list_tail(huge_list);
	double last_node_heading_time = benchmark_heading_time(last_node);

	// then both time should be close
	double epsilon = early_node_heading_time;
	cr_assert_float_eq(
		early_node_heading_time,
		last_node_heading_time,
		epsilon,
		"getting to the first node isn't in constant time");
}


static double benchmark_appending_time(linked_list * list, char * value)
{
	clock_t start = clock();
	list_append(& list, value);
	clock_t end = clock();
	return ((double)(end - start)) / CLOCKS_PER_SEC;
}


Test(linked_list, appending_time_doesnt_depend_of_size)
{
	// given 2 lists, a small one and a big one
	linked_list * small = small_list();
	linked_list * big = big_list();

	// when measuring time it takes to append a new element to the small list...
	double small_list_appending_time = benchmark_appending_time(small, "foo");
	// ... and measuring time it takes to append an element to the bigger list
	double big_list_appending_time = benchmark_appending_time(big, "bar");

	// then both time should be close
	double epsilon = small_list_appending_time;
	cr_assert_float_eq(
		small_list_appending_time,
		big_list_appending_time,
		epsilon,
		"appending elements isn't in constant time");
}


static double benchmark_sizing_time(linked_list * list)
{
	clock_t start = clock();
	list_size(list);
	clock_t end = clock();
	return ((double)(end - start)) / CLOCKS_PER_SEC;
}


Test(linked_list, sizing_whole_list_is_in_constant_time)
{
	// given 2 list, a small one and a big one
	linked_list * small = small_list();
	linked_list * big = big_list();

	// when measuring time it takes to measure the small list...
	double small_list_sizing_time = benchmark_sizing_time(small);
	// ... and measuring time it takes to measure the bigger list
	double big_list_sizing_time = benchmark_sizing_time(big);

	// then both time should be close
	double epsilon = small_list_sizing_time;
	cr_assert_float_eq(
		small_list_sizing_time,
		big_list_sizing_time,
		epsilon,
		"sizing the list isn't in constant time");
}


static double benchmark_prepending_time(linked_list * list, char * value)
{
	clock_t start = clock();
	list_prepend(& list, value);
	clock_t end = clock();
	return ((double)(end - start)) / CLOCKS_PER_SEC;
}


Test(linked_list, prepending_time_doesnt_depend_of_size)
{
	// given 2 lists, a small one and a big one
	linked_list * small = small_list();
	linked_list * big = big_list();

	// when measuring time it takes to prepend a new element to the small list...
	double small_list_prepending_time = benchmark_prepending_time(small, "foo");
	// ... and measuring time it takes to prepend an element to the bigger list
	double big_list_prepending_time = benchmark_prepending_time(big, "bar");

	// then both time should be close
	double epsilon = small_list_prepending_time;
	cr_assert_float_eq(
		small_list_prepending_time,
		big_list_prepending_time,
		epsilon,
		"prepending elements isn't in constant time");
}

#endif /* DO_CONSTANT_TIME_BENCHMARK_TESTS */
