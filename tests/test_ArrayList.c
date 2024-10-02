#include "ArrayList.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void array_list_append_test(void) {
  ArrayList array_list = {0};
  array_list_init(&array_list);

  for (int i = 0; i < 10; i++) {
    void *item = malloc(sizeof(int *));
    memcpy(item, &i, sizeof(int));
    array_list_append(&array_list, item);
    assert(array_list.len == i + 1);
  }

  for (int i = 0; i < 10; i++) {
    assert(*(int *)array_list.items[i] == i);
  }

  array_list_deinit(&array_list);
  array_list_init(&array_list);

  int value = 42;
  void *item = malloc(sizeof(int *));
  memcpy(item, &value, sizeof(int));
  array_list_append(&array_list, item);
  assert(array_list.len == 1);
  assert(*(int *)array_list.items[0] == 42);

  array_list_deinit(&array_list);
}

void array_list_pop_test(void) {
  ArrayList array_list;
  array_list_init(&array_list);

  assert(array_list_pop_or_null(&array_list) == NULL);

  for (int i = 0; i < 10; i++) {
    void *item = malloc(sizeof(int *));
    memcpy(item, &i, sizeof(int));
    array_list_append(&array_list, item);
  }

  for (int i = 9; i >= 0; i--) {
    int *num = (int *)array_list_pop(&array_list);
    assert(array_list.len == i);
    assert(*num == i);
  }

  assert(array_list_pop_or_null(&array_list) == NULL);

  array_list_deinit(&array_list);
}

void array_list_swap_remove_test(void) {
  ArrayList array_list;
  array_list_init(&array_list);

  for (int i = 0; i < 10; i++) {
    void *item = malloc(sizeof(int *));
    memcpy(item, &i, sizeof(int));
    array_list_append(&array_list, item);
  }

  int *num = (int *)array_list_swap_remove(&array_list, 4);
  assert(array_list.len == 9);
  assert(*num == 4);
  assert(*(int *)array_list.items[4] == 9);

  num = (int *)array_list_swap_remove(&array_list, array_list.len - 1);
  assert(array_list.len == 8);
  assert(*num == 8);

  array_list_deinit(&array_list);
}

void array_list_ordered_remove_test(void) {
  ArrayList array_list;
  array_list_init(&array_list);

  for (int i = 0; i < 10; i++) {
    void *item = malloc(sizeof(int *));
    memcpy(item, &i, sizeof(int));
    array_list_append(&array_list, item);
  }

  int *num = (int *)array_list_ordered_remove(&array_list, 4);
  assert(array_list.len == 9);
  assert(*num == 4);

  for (int i = 0; i < 4; i++) {
    assert(*(int *)array_list.items[i] == i);
  }

  for (int i = 4; i < array_list.len; i++) {
    assert(*(int *)array_list.items[i] == i + 1);
  }

  array_list_deinit(&array_list);
}

int main(void) {
  array_list_append_test();
  array_list_pop_test();
  array_list_swap_remove_test();
  array_list_ordered_remove_test();

  printf("All tests passed successfully!\n");
  return 0;
}
