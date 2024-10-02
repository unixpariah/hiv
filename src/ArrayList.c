#include "ArrayList.h"
#include "stdio.h"
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

void array_list_init(ArrayList *array_list) {
  array_list->capacity = 0;
  array_list->len = 0;
  array_list->items = NULL;
}

enum Result array_list_init_capacity(ArrayList *array_list, uint32_t capacity) {
  void **items = (void **)malloc(sizeof(void *) * capacity);
  if ((void *)items == NULL) {
    return OOM;
  }

  array_list->capacity = capacity;
  array_list->len = 0;
  array_list->items = items;

  return OK;
}

uint32_t grow_capacity(uint32_t current, uint32_t minimum) {
  uint32_t new_capacity = current;
  while (new_capacity < minimum) {
    new_capacity += new_capacity / 2 + 8;
  }
  return new_capacity;
}

enum Result array_list_ensure_total_capacity_precise(ArrayList *array_list,
                                                     uint32_t new_capacity) {
  if (array_list->capacity >= new_capacity) {
    return OK;
  }

  void **new_items = (void **)realloc((void *)array_list->items,
                                      new_capacity * sizeof(void *));
  if (!new_items) {
    return OOM;
  }

  array_list->capacity = new_capacity;
  array_list->items = new_items;

  return OK;
}

enum Result array_list_ensure_total_capacity(ArrayList *array_list,
                                             uint32_t new_capacity) {
  if (array_list->capacity >= new_capacity) {
    return OK;
  }

  uint32_t better_capacity = grow_capacity(array_list->capacity, new_capacity);
  return array_list_ensure_total_capacity_precise(array_list, better_capacity);
}

void **array_list_add_one_assume_capacity(ArrayList *array_list) {
  assert(array_list->len < array_list->capacity);
  array_list->len++;
  return &array_list->items[array_list->len - 1];
}

void **array_list_add_one(ArrayList *array_list) {
  uint32_t new_len = array_list->len + 1;
  if (array_list_ensure_total_capacity(array_list, new_len) == OOM) {
    return NULL;
  };
  return array_list_add_one_assume_capacity(array_list);
}

enum Result array_list_append(ArrayList *array_list, void *item) {
  void **new_item_ptr = array_list_add_one(array_list);
  if ((void *)new_item_ptr == NULL) {
    return OOM;
  }

  *new_item_ptr = item;

  return OK;
}

void array_list_append_assume_capacity(ArrayList *array_list, void *item) {
  void **new_item_ptr = array_list_add_one_assume_capacity(array_list);
  *new_item_ptr = item;
}

void *array_list_pop(ArrayList *array_list) {
  void *item = array_list->items[array_list->len - 1];
  array_list->items[array_list->len - 1] = 0;

  array_list->len--;

  return item;
}

void *array_list_pop_or_null(ArrayList *array_list) {
  if (array_list->len == 0) {
    return NULL;
  }

  return array_list_pop(array_list);
}

void *array_list_swap_remove(ArrayList *array_list, uint32_t index) {
  assert(index < 0 || index >= array_list->len);

  if (array_list->len - 1 == index) {
    return array_list_pop(array_list);
  }

  void *old_item = array_list->items[index];
  array_list->items[index] = array_list_pop(array_list);

  return old_item;
}

void *array_list_ordered_remove(ArrayList *array_list, uint32_t index) {
  assert(index < 0 || index < array_list->len);

  if (array_list->len - 1 == index) {
    return array_list_pop(array_list);
  }

  void *old_item = array_list->items[index];
  for (uint32_t i = index; i < array_list->len - 1; i++) {
    array_list->items[i] = array_list->items[i + 1];
  }

  array_list->len--;

  return old_item;
}

void array_list_deinit(ArrayList *array_list) {
  for (uint32_t i = 0; i < array_list->len; i++) {
    free(array_list->items[i]);
  }
  free((void *)array_list->items);
  array_list->items = NULL;
  array_list->capacity = 0;
  array_list->len = 0;
}
