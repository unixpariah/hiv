#include <stdint.h>

typedef struct Node Node;

typedef struct Node {
  Node *next;
  void *data;
} Node;

void node_insert_after(Node *node, Node *new_node);

Node *node_remove_next(Node *node);

Node *node_find_last(Node *node);

uint64_t node_count_children(Node *node);

void node_reverse(Node **indirect);

typedef struct {
  Node *first;
} SinglyLinkedList;

void singly_linked_list_prepend(SinglyLinkedList *list, Node *new_node);

void singly_linked_list_remove(SinglyLinkedList *list, Node *node);

Node *singly_linked_list_pop_first(SinglyLinkedList *list);

uint64_t singly_linked_list_len(SinglyLinkedList *list);
