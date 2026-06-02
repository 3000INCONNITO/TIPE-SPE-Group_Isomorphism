

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

#endif