#include "ArrayList.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int array_list_append_test(void) {
  ArrayList array_list = {0};
  array_list_init(&array_list);

  for (uint32_t i = 0; i < 10; i++) {
    uint32_t *item = malloc(sizeof(uint32_t));
    *item = i;
    array_list_append(&array_list, item);
    if (array_list.len != i + 1) {
      return -1;
    }
  }

  for (uint32_t i = 0; i < 10; i++) {
    if (*(uint32_t *)array_list.items[i] != i) {
      return -1;
    }
  }

  for (uint32_t i = 0; i < array_list.len; i++) {
    free(array_list.items[i]);
  }
  array_list_deinit(&array_list);
  array_list_init(&array_list);

  uint32_t *item = malloc(sizeof(uint32_t));
  *item = 42;
  array_list_append(&array_list, item);
  if (array_list.len != 1 || *(uint32_t *)array_list.items[0] != 42) {
    return -1;
  }

  for (uint32_t i = 0; i < array_list.len; i++) {
    free(array_list.items[i]);
  }
  array_list_deinit(&array_list);
  return 0;
}

int array_list_pop_test(void) {
  ArrayList array_list;
  array_list_init(&array_list);

  if (array_list_pop_or_null(&array_list) != NULL) {
    return -1;
  }

  for (uint32_t i = 0; i < 10; i++) {
    uint32_t *item = malloc(sizeof(uint32_t));
    *item = i;
    array_list_append(&array_list, item);
  }

  for (uint32_t i = 10; i > 0; i--) {
    uint32_t *num = array_list_pop(&array_list);
    if (array_list.len != i - 1 || *num != i - 1) {
      return -1;
    }
    free(num);
  }

  if (array_list_pop_or_null(&array_list) != NULL) {
    return -1;
  }

  for (uint32_t i = 0; i < array_list.len; i++) {
    free(array_list.items[i]);
  }
  array_list_deinit(&array_list);
  return 0;
}

int array_list_swap_remove_test(void) {
  ArrayList array_list;
  array_list_init(&array_list);

  for (uint32_t i = 0; i < 10; i++) {
    uint32_t *item = malloc(sizeof(uint32_t));
    *item = i;
    array_list_append(&array_list, item);
  }

  uint32_t *num = array_list_swap_remove(&array_list, 4);
  if (array_list.len != 9 || *num != 4 ||
      *(uint32_t *)array_list.items[4] != 9) {
    return -1;
  }
  free(num);

  num = (uint32_t *)array_list_swap_remove(&array_list, array_list.len - 1);
  if (array_list.len != 8 || *num != 8) {
    return -1;
  }
  free(num);

  for (uint32_t i = 0; i < array_list.len; i++) {
    free(array_list.items[i]);
  }
  array_list_deinit(&array_list);
  return 0;
}

int array_list_ordered_remove_test(void) {
  ArrayList array_list;
  array_list_init(&array_list);

  for (uint32_t i = 0; i < 10; i++) {
    uint32_t *item = malloc(sizeof(uint32_t));
    *item = i;
    array_list_append(&array_list, item);
  }

  uint32_t *num = array_list_ordered_remove(&array_list, 4);
  if (array_list.len != 9 || *num != 4) {
    return -1;
  }
  free(num);

  for (uint32_t i = 0; i < 4; i++) {
    if (*(uint32_t *)array_list.items[i] != i) {
      return -1;
    }
  }
  for (uint32_t i = 4; i < array_list.len; i++) {
    if (*(uint32_t *)array_list.items[i] != i + 1) {
      return -1;
    }
  }

  for (uint32_t i = 0; i < array_list.len; i++) {
    free(array_list.items[i]);
  }
  array_list_deinit(&array_list);
  return 0;
}

int array_list_init_capacity_test(void) {
  ArrayList array_list = {0};
  array_list_init_capacity(&array_list, 10);

  if (array_list.capacity != 10) {
    return -1;
  }

  for (uint32_t i = 0; i < array_list.capacity; i++) {
    uint32_t *item = malloc(sizeof(uint32_t));
    *item = i;
    array_list_append_assume_capacity(&array_list, item);
  }

  if (array_list.len != array_list.capacity) {
    return -1;
  }

  uint32_t *new_value = malloc(sizeof(uint32_t));
  *new_value = 100;
  array_list_append(&array_list, new_value);

  if (array_list.len != 11 || array_list.capacity <= 10) {
    return -1;
  }

  if (*(uint32_t *)array_list.items[10] != 100) {
    return -1;
  }

  for (uint32_t i = 0; i < array_list.len; i++) {
    free(array_list.items[i]);
  }
  array_list_deinit(&array_list);

  return 0;
}

int main(void) {
  uint32_t passes = 0;

  if (array_list_append_test() == -1) {
    printf("Passed %d tests\n", passes);
    return EXIT_FAILURE;
  }
  passes++;

  if (array_list_pop_test() == -1) {
    printf("Passed %d tests\n", passes);
    return EXIT_FAILURE;
  }
  passes++;

  if (array_list_swap_remove_test() == -1) {
    printf("Passed %d tests\n", passes);
    return EXIT_FAILURE;
  }
  passes++;

  if (array_list_ordered_remove_test() == -1) {
    printf("Passed %d tests\n", passes);
    return EXIT_FAILURE;
  }
  passes++;

  if (array_list_init_capacity_test() == -1) {
    printf("Passed %d tests\n", passes);
    return EXIT_FAILURE;
  }
  passes++;

  printf("Passed %d tests\n", passes);
  return EXIT_SUCCESS;
}
