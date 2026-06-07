
#include "Garbage_Collector/garbage_collector.h"

data_list* create_decomp_tabl(int n) {
	data_list* tabl = malloc(n*sizeof(data_list));
	for (int i = 0; i < n; i++) {
		tabl[i] = create_empty_data_list();
	}

	return tabl;
}

// void special_add(data_list* tabl, int i, int g, int p, gbg_collector coll) {
// 	push_data_list(tabl[i], g, p);
// 	push_gbg_collector(tabl[i]->head, coll);
// 	node_data* elt_addr = &(tabl[i]->head);

// 	// printf("Address of new head in tabl: %d\n", &(tabl[i]->head));
// 	// printf("Address stored in collector: %d\n", elt_addr);
// }


// void append_data_list_from_ptr(data_list dlist, node_data* elt_ptr) {
// 	if (dlist->length == 0) {
// 		dlist->head = *elt_ptr;
// 		dlist->tail = *elt_ptr;
// 	} else {
// 		dlist->tail->next = *elt_ptr;
// 		dlist->tail = *elt_ptr;
// 	}
// 	dlist->length++;
// }

// tabl[i]->tail = tabl[j]->head
void add_link(data_list* tabl, int i, int j, int g, int p, gbg_collector coll) {
	assert(i != j);
	node_data* new_tail_addr = malloc(sizeof(node_data));
	*new_tail_addr = new_data_node(g, p);

	append_data_list_from_ptr(tabl[i], new_tail_addr);
	
	if (j >= 0) {
		tabl[i]->tail->next = tabl[j]->head;
	}
	push_gbg_collector(new_tail_addr, coll);

	printf("Added value at index %d: (%d, %d)\n", i, g, p);
	printf("Address of new tail in tabl: %d\n", new_tail_addr);
}

void free_singl_gbg_node(gbg_collector coll) {
	gbg_node old_head = coll->head;
	gbg_node new_head = old_head->next;
	free(*(old_head->to_free));
	// free(old_head);
	coll->head = new_head;
}

int main() {
	int n = 5;
	data_list* tabl = create_decomp_tabl(n);
	gbg_collector collector = create_empty_gbg_collector();

	append_data_in_tabl_only(tabl, n, 0, 0, 0, collector);
	append_data_in_tabl_only(tabl, n, 0, -1, 0, collector);
	append_data_and_link_in_tabl(tabl, n, 1, 0, 1, 0, collector);
	append_data_and_link_in_tabl(tabl, n, 2, 1, 2, 0, collector);

	show_values_data_list(tabl[2]);

	free_decomp_table(tabl, n, collector);

	return 0;
}
