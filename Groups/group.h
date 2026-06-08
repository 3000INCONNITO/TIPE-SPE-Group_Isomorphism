

#ifndef GROUP_H
#define GROUP_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "../Gen_Table/gen_table.h"

typedef struct group {
	char* name;
	int n;
	int** table;
} group_t;
typedef group_t* group;

int *zero_array(int n);
void group_name(group_t *g, char *s);
group group_empty(int n);
void group_free(group g);
void group_print(group g);
group from_matrix(int **m);
group copy_group(group g);
int calculate(group g, int x, int y);
bool is_neutral(group g, int x);
int get_neutral(group  g);

typedef long long int ll;
typedef struct cpr_group_s {
	int rank;
	ll n;
	int* ivf;
	int* strides;
} cpr_group_t;
typedef cpr_group_t* cpr_group;

cpr_group create_cpr_group(int n, int rank, int* ivf);
void cpr_group_free(cpr_group cpr_grp);
void print_decomp(cpr_group c_grp, int x);
int* cpr_group_encode(cpr_group c_grp, int x);
int cpr_group_decode(cpr_group c_grp, int* decomp);
int cpr_group_calculate(cpr_group cpr_grp, int x, int y);

#endif