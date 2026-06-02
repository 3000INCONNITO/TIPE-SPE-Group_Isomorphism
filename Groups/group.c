

#include "group.h"

int *zero_array(int n) {
	int *res = malloc(n * sizeof(int));
	for (int i = 0; i < n; i++) {
		res[i] = 0;
	}
	return res;
}

void group_name(group_t *g, char *s) {
	g->name = s;
}

group group_empty(int n) {
	group new_empty_group = malloc(sizeof(group_t));
	new_empty_group->n = n;
	new_empty_group->table = malloc(n * sizeof(int *));
	for (int i = 0; i < n; i++) {
		new_empty_group->table[i] = zero_array(n);
	}
	return new_empty_group;
}

void group_free(group g) {
	assert(g != NULL && "[-] NULL POINTER exception.\n");
	int n = g->n;
	for (int i = 0; i < n; i++) {
		free(g->table[i]);
	}
	free(g->table);
	free(g);
}


void group_print(group g) {
	assert(g != NULL && "[-] NULL POINTER exception.\n");
  int n = g->n;
	printf("Groupe %s d'ordre %d\n", g->name, n);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			printf("%d ", g->table[i][j]);
		}
		printf("\n");
	}
}

group from_matrix(int **m) {
	assert(m != NULL && "[-] NULL POINTER exception.\n");
	int n = sizeof(*m[0]) / sizeof(int);
	group res = group_empty(n);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			res->table[i][j] = m[i][j];
		}
	}
	return res;
}

group copy_group(group g) {
	assert(g != NULL && "[-] NULL POINTER exception.\n");
	int n = g->n;
	group copy = group_empty(n);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			copy->table[i][j] = g->table[i][j];
		}
	}
	return copy;
}

int calculate(group g, int x, int y) {
	assert(y >= 0);
	if (x == -1) {
		return y;
	}
	return g->table[x][y];
}

bool is_neutral(group g, int x) {
	assert(g != NULL);
	assert(x >= 0 && x < g->n);
	int res;
	if (x < g->n - 1) {
		res = calculate(g, x, x+1);
		return (res == x+1);
	} else {
		res = calculate(g, x, 0);
		return (res == 0);
	}
}

int get_neutral(group  g) {
	assert(g != NULL);
	for (int i = 0; i < g->n; i++) {
		if (is_neutral(g, i)) {
			return i;
		}
	}
	printf("Erreur.\n");
	return -1;
}