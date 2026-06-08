#include "gen_table.h"

data create_data(int g, int p) {
	data d = malloc(sizeof(data_t));
	d->g = g;
	d->p = p;
	return d;
}

node_data new_data_node(int g, int p) {
	node_data node = malloc(sizeof(node_data_t));
	node->g = g;
	node->p = p;
	node->next = NULL;
	return node;
}

data_list create_empty_data_list() {
	data_list new_data_list = malloc(sizeof(data_list_t));
	new_data_list->head = NULL;
	new_data_list->tail = NULL;
	new_data_list->length = 0;
	return new_data_list;
}

data_list create_example_data_list(int n) {
	data_list example = create_empty_data_list();
	for (int i = 0; i < n; i++) {
		push_data_list(example, i, i);
	}
	return example;
}

data_list create_single_data_list(int g) {
	data_list single = create_empty_data_list();
	push_data_list(single, g, 1);
	return single;
}

data_list deep_copy_data_list(data_list dlist) {
	data_list copy = create_empty_data_list();
	for (node_data c=dlist->head; c != NULL; c=c->next) {
		append_data_list(copy, c->g, c->p);
	}

	return copy;
}

void del_copy_from(data_list* d1, data_list d2) {
	assert(d1 != NULL && d2 != NULL);
	free_data_list(*d1);
	*d1 = deep_copy_data_list(d2);
}

bool is_empty_data_list(data_list dlist) {
	assert(dlist != NULL);
	return (dlist->length == 0 && dlist->head == NULL);
}

void show_values_data_list(data_list dlist) {
	assert(dlist != NULL);
	for (node_data c = dlist->head; c != NULL; c=c->next) {
		printf("(%d,%d) -> ", c->g, c->p);
	}
	printf("END\n");
}

void push_data_list(data_list dlist, int g, int p) {
	// printf("Adding: (%d, %d)\n", g, p);

	node_data new_head = new_data_node(g, p);
	node_data old_head = dlist->head;
	new_head->next = old_head;
	dlist->head = new_head;
	dlist->length += 1;

	if (dlist->length == 1) {
		dlist->tail = dlist->head;
	}
}

void append_data_list(data_list dlist, int g, int p) {
	if (dlist->length <= 0) {
		push_data_list(dlist, g, p);
	} else {
		node_data new_tail = new_data_node(g, p);
		dlist->tail->next = new_tail;
		dlist->tail = new_tail;
		dlist->length += 1;
	}
}

data pop_data_list(data_list dlist) {
	assert(dlist != NULL && !is_empty_data_list(dlist));
	node_data old_head = dlist->head;
	node_data new_head = old_head->next;
	dlist->head = new_head;

	data res = create_data(old_head->g, old_head->p);
	free(old_head);
	dlist->length--;
	return res;
}

node_data free_get_next_node_data(node_data node) {
	assert(node != NULL);
	node_data next = node->next;
	free(node);
	return next;
}

void free_data_list(data_list dlist) {
	assert(dlist != NULL);
	node_data curr = dlist->head;
	while (curr != NULL) {
		curr = free_get_next_node_data(curr);
	}

	free(dlist);
}

group_data create_empty_group_data(int n) {
	group_data res = malloc(sizeof(group_data_t));
	res->n = n;
	res->initialised = false;


	res->contains = calloc(n, sizeof(bool));
	res->order = malloc(n*sizeof(int));
	for (int i = 0; i < n; i++) {
		res->order[i] = -1;
	}

	res->grp_elts = create_empty_list();
	res->generators = create_empty_data_list();

	res->decomps = malloc(n*sizeof(data_list));
	for (int i = 0; i < n; i++) {
		res->decomps[i] = create_empty_data_list();
	}

	return res;
}

void init_empty_group_data(group_data gd, int neutral) {
	assert(gd != NULL && neutral >= 0 && neutral < gd->n);
	assert(!gd->initialised);
	assert(gd->generators->length == 0 && gd->grp_elts->length == 0);
	push(gd->grp_elts, neutral);
	gd->contains[neutral] = true;
	gd->initialised = true;
}

void free_group_data(group_data tr) {
	assert(tr != NULL);
	for (int i = 0; i < tr->n; i++) {
		free_data_list(tr->decomps[i]);
	}
	free(tr->decomps);
	free_list(tr->grp_elts);
	free_data_list(tr->generators);
	free(tr->order);
	free(tr->contains);
	free(tr);
}

void show_num_elts(group_data tr) {
	int num_elts = 0;
	for (int i = 0; i < tr->n; i++) {
		if (tr->contains[i]) {
			num_elts++;
		}
	}
	printf("Nombre d'éléménts: %d\n", num_elts);
}

void show_rel(group_data tr) {
	printf("Décompositions :\n");
	assert(tr != NULL && tr->decomps != NULL);
	for (int i = 0; i < tr->n; i++) {
		if (tr->decomps[i]->length >= 1) {	
			printf("(%d,%d): ", i, tr->decomps[i]->length);
			show_values_data_list(tr->decomps[i]);
		}
	}
}

void show_contains(group_data tr) {
	assert(tr != NULL && tr->contains != NULL);
	for (int i = 0; i < tr->n; i++) {
		printf("Has %d: ", i);
		if (tr->contains[i]) {
			printf("TRUE\n");
		} else {
			printf("FALSE\n");
		}
	}
}

void show_grp_elts(group_data tr) {
	assert(tr != NULL && tr->grp_elts != NULL);
	show_values(tr->grp_elts);
}

void show_generators(group_data tr) {
	assert(tr != NULL && tr->generators != NULL);
	printf("Générateur(s): ");
	show_values_data_list(tr->generators);
}