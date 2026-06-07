#ifndef GARBAGE_COLLECTOR_H
#define GARBAGE_COLLECTOR_H

#include "../Gen_Table/gen_table.h"

typedef struct gbg_node_s {
	node_data* to_free;
	struct gbg_node_s* next;
} gbg_node_t;
typedef gbg_node_t* gbg_node;

typedef struct gbg_collector_s {
	gbg_node head;
	int size;
} gbg_collector_t;
typedef gbg_collector_t* gbg_collector;

gbg_node create_gbg_node(node_data* nd_addr);
void free_gbg_node(gbg_node head_gbg_node);

gbg_collector create_empty_gbg_collector(void);
void push_gbg_collector(node_data* nd_addr, gbg_collector collector);
void free_gbg_collector(gbg_collector collector);
void print_gbg_collector(gbg_collector collector);

void free_decomp_table(data_list* tabl, int n, gbg_collector coll);

// New functions to append in decomp table.
void append_data_list_from_ptr(data_list dlist, node_data* ptr);
void append_data_in_tabl_only(data_list* tabl, int n, int i, int g, int p, gbg_collector coll);
void append_data_and_link_in_tabl(data_list* tabl, int n, int i, int j, int g, int p, gbg_collector coll);

void free_gbc_gdata(group_data gd, gbg_collector gbc);


#endif