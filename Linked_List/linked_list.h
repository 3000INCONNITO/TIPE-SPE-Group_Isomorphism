

#ifndef LL
#define LL

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

typedef struct node {
  int val;
  struct node* next;
} node_t;

typedef struct list {
  node_t* head;
  node_t* tail;
  int length;
} list_t;
typedef list_t* list;

node_t* new_node(int n);

list_t* create_empty_list();
list_t* create_example(int n);
list_t* create_single(int n);

list_t* deep_copy(list_t* l);

bool is_empty(list_t* linked);

void show_list(list_t* linked);
void show_values(list_t* linked);

void push(list_t* linked, int n);
void append(list_t* linked, int n);
void insert(list_t* linked, int index, int x);
int pop(list_t* linked);

node_t* free_get_next(node_t* node);
void free_list(list_t* linked);

#endif