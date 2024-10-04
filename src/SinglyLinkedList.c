#include <SinglyLinkedList.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

void node_insert_after(Node *node, Node *new_node) {
  new_node->next = node->next;
  node->next = new_node;
}

Node *node_remove_next(Node *node) {
  Node *next_node = node->next;
  if (next_node == NULL) {
    return NULL;
  }

  node->next = next_node->next;
  return next_node;
}

Node *node_find_last(Node *node) {
  Node *current_node = node;
  while (true) {
    if (current_node->next == NULL) {
      return current_node;
    }
    current_node = current_node->next;
  }
}

uint64_t node_count_children(Node *node) {
  uint64_t count = 0;
  Node *current_node = node->next;
  while (current_node != NULL) {
    count++;
    current_node = current_node->next;
  }

  return count;
}

void node_reverse(Node **indirect) {
  if (*indirect == NULL) {
    return;
  }
  Node *current = *indirect;
  while (true) {
    Node *next = current->next;
    if (next == NULL) {
      break;
    }
    current->next = next->next;
    next->next = *indirect;
    *indirect = next;
  }
}

void singly_linked_list_prepend(SinglyLinkedList *list, Node *new_node) {
  new_node->next = list->first;
  list->first = new_node;
}

void singly_linked_list_remove(SinglyLinkedList *list, Node *node) {
  if (list->first == node) {
    list->first = node->next;
  } else {
    Node *current_node = list->first;
    while (current_node != node) {
      current_node = current_node->next;
    }
    current_node->next = node->next;
  }
}

Node *singly_linked_list_pop_first(SinglyLinkedList *list) {
  Node *first = list->first;
  if (first == NULL) {
    return NULL;
  }
  list->first = first->next;
  return first;
}

uint64_t singly_linked_list_len(SinglyLinkedList *list) {
  Node *node = list->first;
  if (node == NULL) {
    return 0;
  }

  return 1 + node_count_children(node);
}
