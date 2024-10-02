#ifndef ARRAY_LIST_H
#define ARRAY_LIST_H

#include <stdint.h>

enum Result {
  OK = 0,
  OOM = 1,
};

typedef struct {
  uint32_t capacity;
  uint32_t len;
  void **items;
} ArrayList;

/// Must be freed with array_list_deinit.
void array_list_init(ArrayList *array_list);

/// Initialize with capacity.
/// Must be freed with array_list_deinit.
enum Result array_list_init_capacity(ArrayList *array_list, uint32_t capacity);

/// Extend list by 1 element.
/// Allocates more memory if necessary.
enum Result array_list_append(ArrayList *array_list, void *item);

/// Extend list by 1 element.
/// Asserts that list can hold additional item.
void array_list_append_assume_capacity(ArrayList *array_list, void *item);

/// Increase length by 1, returning pointer to the new item.
/// Asserts that list can hold additional item.
void **array_list_add_one_assume_capacity(ArrayList *array_list);

/// Increase length by 1, returning pointer to the new item.
/// Allocates more memory if necessary.
void **array_list_add_one(ArrayList *array_list);

/// Removes the element at the specified index and returns it.
/// The empty slot is filled from the end of the list.
/// This operation is O(1).
/// Asserts that the list is not empty.
/// Asserts that the index is in bounds.
/// Returned item must be freed by caller.
void *array_list_swap_remove(ArrayList *array_list, uint32_t index);

/// Remove the element at index `i`, shift elements after index
/// `i` forward, and return the removed element.
/// This operation is O(N).
/// This preserves item order. Use `swapRemove` if order preservation is not
/// important. Asserts that the index is in bounds. Asserts that the list is not
/// empty.
/// Returned item must be freed by caller.
void *array_list_ordered_remove(ArrayList *array_list, uint32_t index);

/// Remove and return the last element from the list.
/// Asserts that the list is not empty.
/// Returned item must be freed by caller.
void *array_list_pop(ArrayList *array_list);

/// Remove and return the last element from the list, or
/// return `null` if list is empty.
/// Returned item must be freed by caller.
void *array_list_pop_or_null(ArrayList *array_list);

/// Release all allocated memory
void array_list_deinit(ArrayList *array_list);

#endif // ARRAY_LIST_H
