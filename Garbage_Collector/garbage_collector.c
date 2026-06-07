#include "garbage_collector.h"

gbg_node create_gbg_node(node_data* nd_addr) {
	gbg_node res = malloc(sizeof(gbg_node_t));
	res->to_free = nd_addr;
	res->next = NULL;

	return res;
}

void free_gbg_node(gbg_node head_gbg_node) {
	if (head_gbg_node == NULL) return;
	gbg_node next = head_gbg_node->next;
	free(*(head_gbg_node->to_free));
	free(head_gbg_node->to_free);
	free(head_gbg_node);

	free_gbg_node(next);
}

gbg_collector create_empty_gbg_collector(void) {
	gbg_collector res = malloc(sizeof(gbg_collector_t));
	res->head = NULL;
	res->size = 0;
}

void push_gbg_collector(node_data* nd_addr, gbg_collector collector) {
	gbg_node new_head = create_gbg_node(nd_addr);
	new_head->next = collector->head;
	collector->head = new_head;
	collector->size++;
}

void free_gbg_collector(gbg_collector collector) {
	free_gbg_node(collector->head);
	free(collector);
}

void print_gbg_collector(gbg_collector collector) {
	assert(collector != NULL);
	printf("To free: %d\n", collector->size);
}

void free_decomp_table(data_list* tabl, int n, gbg_collector coll) {
	free_gbg_collector(coll);
	for (int i = 0; i < n; i++) {
		free(tabl[i]);
	}
	free(tabl);
}

void append_data_list_from_ptr(data_list dlist, node_data* ptr) {
	assert(dlist != NULL);
	if (dlist->length == 0) {
		dlist->head = *ptr;
		dlist->tail = *ptr;
	} else {
		dlist->tail->next = *ptr;
		dlist->tail = *ptr;
	}
	dlist->length++;
}

void append_data_in_tabl_only(data_list* tabl, int n, int i, int g, int p, gbg_collector coll) {
	assert(tabl != NULL && i >= 0 && i < n);
	
	// Allocate fresh memory in the heap.
	node_data* ptr = malloc(sizeof(node_data));
	*ptr = new_data_node(g, p);

	append_data_list_from_ptr(tabl[i], ptr);

	push_gbg_collector(ptr, coll);

	// printf("Added value at index %d: (%d, %d)\n", i, g, p);
	// printf("Address of new tail in tabl: %p\n", ptr);
}

void append_data_and_link_in_tabl(data_list* tabl, int n, int i, int j, int g, int p, gbg_collector coll) {
	assert(tabl != NULL && i != j);
	assert(i >= 0 && i < n && j >= 0 && j < n);

	// Allocate fresh memory in the heap.
	node_data* ptr = malloc(sizeof(node_data));
	*ptr = new_data_node(g, p);

	append_data_list_from_ptr(tabl[i], ptr);

	tabl[i]->tail->next = tabl[j]->head;

	push_gbg_collector(ptr, coll);

	
	// printf("Added value at index %d: (%d, %d)\n", i, g, p);
	// printf("Address of new tail in tabl: %p\n", ptr);
}

void free_gbc_gdata(group_data gd, gbg_collector gbc) {
	// free_gbg_collector(gbc);
	// for (int i = 0; i < gd->n; i++) {
	// 	free(gd->decomps[i]);
	// }
	// free(gd->decomps);
	free_decomp_table(gd->decomps, gd->n, gbc);
	free_list(gd->grp_elts);
	free_data_list(gd->generators);
	free(gd->order);
	free(gd->contains);
	free(gd);
}