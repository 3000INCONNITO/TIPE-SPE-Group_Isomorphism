

#ifndef GEN_TBL
#define GEN_TBL

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "../Linked_List/linked_list.h"

typedef struct data_s {
	int g;
	int p;
} data_t;
typedef data_t* data;

typedef struct node_data_s {
	int g;
	int p;
	struct node_data_s* next;
} node_data_t;

typedef node_data_t* node_data;

typedef struct data_list_s {
  node_data head;
  node_data tail;
  int length;
} data_list_t;
typedef data_list_t* data_list;

node_data new_data_node(int g, int p);
bool is_empty_data_list(data_list dlist);

data_list create_empty_data_list();
data_list create_example_data_list(int n);
data_list create_single_data_list(int n);
data_list deep_copy_data_list(data_list dlist);
void del_copy_from(data_list* d1, data_list d2);

void show_values_data_list(data_list dlist);
void push_data_list(data_list dlist, int g, int p);
void append_data_list(data_list dlist, int g, int p);
data pop_data_list(data_list dlist);

node_data free_get_next_node_data(node_data node);
void free_data_list(data_list dlist);

typedef struct group_data_s {
	int n;
	bool initialised;

	bool* contains;
	list grp_elts;
	
	int* order;
	data_list generators;

	data_list* decomps;
} group_data_t;
typedef group_data_t* group_data;

group_data create_empty_group_data(int n);
void init_empty_group_data(group_data gd, int neutral);
void free_group_data(group_data tr);

void show_num_elts(group_data tr);
void show_rel(group_data tr);
void show_contains(group_data tr);
void show_grp_elts(group_data tr);
void show_generators(group_data tr);

#endif