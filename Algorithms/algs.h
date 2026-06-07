

#ifndef ALGS_H
#define ALGS_H

#include "../Groups/group.h"
#include "../Gen_Table/gen_table.h"
#include "../Garbage_Collector/garbage_collector.h"
#include "../Linked_List/linked_list.h"
#include "snf.h"
#include "algs.h"
#include <stdbool.h>

int find_neutral(group g);
int fast_exp(group g, int x, int neutral, int n);

int calculate_next_gen_and_rel(group g, group_data gd, int new_gen, int neutral);
int calculate_next_gen_and_rel_fast(group g, group_data gd, gbg_collector coll, int new_gen, int neutral);

group_data calculate_group_data(group g);
group_data calculate_group_data_fast(group g, gbg_collector trash);

void check_group_data(group g, group_data gd, int neutral);

void print_arr(int* arr, int n);

matrix get_trig_rel_table(group g, int neutral, group_data gd);

typedef struct {
	int n;
	int* order;
	int* elts;
} basis_s;
typedef basis_s* basis;

basis create_empty_basis(int n);
void free_basis(basis b);
void check_basis(group g, basis b, int neutral);
void show_basis(basis b);

basis get_basis_from_data(group g, group_data gd, int neutral, matrix D, matrix V_inv);
basis get_basis_from_rel(group g, group_data gd, int neutral);
basis get_basis(group g);

void iterate_decomps(int* decomps, int dim, int i, int* orders);
bool are_isomorphic(group g1, group g2);
int* calculate_isomorphism(group g1, group g2);

bool check_isomorphism(group g1, group g2, int* iso);
#endif