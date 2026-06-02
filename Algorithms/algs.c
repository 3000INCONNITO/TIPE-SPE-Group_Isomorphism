

#include "algs.h"

int find_neutral(group g) {
	int next = 0;
	for (int i = 0; i < g->n; i++) {
		next = (i+1) % g->n;
		if (calculate(g, i, next) == next) {
			return i;
		}
	}
	printf("[-] Erreur.\n");
	return -1;
}

int fast_exp(group g, int x, int neutral, int n) {
	assert(g != NULL && x >= 0 && x < g->n);
	assert(is_neutral(g, neutral));
	if (n >= 0) {
		int res = neutral;
		int a = x;
		int p = (n % g->n);
		while (p > 0) {
			if (p % 2 == 1) {
				res = calculate(g, res, a);
			}
			a = calculate(g, a, a);
			p /= 2;
		}

		return res;
	} else {
		int p = (n % g->n) + g->n;
		return fast_exp(g, x, neutral, p);
	}
}

// Calcule les relations étant donné un nouveau générateur.
int calculate_next_gen_and_rel(group g, group_data gd, int new_gen, int neutral) {
	assert(g != NULL && gd != NULL);
	assert(new_gen >= 0);

	if (!gd->initialised) {
		init_empty_group_data(gd, neutral);
	}
	list new_grp_elts = create_empty_list();
	int curr = new_gen;
	int k = 1;
	int to_add, x;

	while (!gd->contains[curr]) {
		for (node_t* node = gd->grp_elts->head; node != NULL; node = node->next) {
			x = node->val;
			to_add = calculate(g, curr, x);

			if (!gd->contains[curr]) {
				push(new_grp_elts, curr);
				push_data_list(gd->decomps[curr], new_gen, k);
				gd->contains[curr] = true;
			}

			if (!gd->contains[to_add]) {
				push(new_grp_elts, to_add);
				del_copy_from(&gd->decomps[to_add], gd->decomps[x]);
				push_data_list(gd->decomps[to_add], new_gen, k);
				gd->contains[to_add] = true;
			}
		}

		k++;
		curr = calculate(g, curr, new_gen);
	}

	push_data_list(gd->generators, new_gen, k);

	int to_push;
	while(new_grp_elts->length > 0) {
		to_push = pop(new_grp_elts);
		push(gd->grp_elts, to_push);
	}
	free_list(new_grp_elts);

	int next_gen = 0;
	while (next_gen < gd->n && gd->contains[next_gen]) {
		next_gen++;
	}

	return next_gen;
}

group_data calculate_group_data(group g) {
	int neutral = find_neutral(g);

	group_data gd = create_empty_group_data(g->n);
	init_empty_group_data(gd, neutral);

	int gen = 0;

	while (gen < g->n && gd->grp_elts->length < g->n) {
		gen = calculate_next_gen_and_rel(g, gd, gen, neutral);
	}

	return gd;
}

void check_group_data(group g, group_data gd, int neutral) {
	for (int i = 0; i < g->n; i++) {
		int res = neutral;
		for (node_data c = gd->decomps[i]->head; c != NULL; c=c->next) {
			int val = fast_exp(g, c->g, neutral, c->p);
			res = calculate(g, res, val);
		}
		if (i != res) {
			printf("[-] ERROR: %d != %d.\n", i, res);
		}
		assert(res == i);
	}
}

void print_arr(int* arr, int n) {
	for (int i = 0; i < n; i++) {
		printf("%d ", arr[i]);
	}
	printf("\n");
}


matrix get_trig_rel_table(group g, int neutral, group_data gd) {
	assert(gd != NULL && gd->generators != NULL);

	int t = gd->generators->length;
	matrix trig_rel = mat_alloc(t);

	int *col_of = malloc((size_t)g->n * sizeof(int));
	assert(col_of != NULL);
	for (int x = 0; x < g->n; x++) col_of[x] = -1;

	int col = 0;
	for (node_data gen = gd->generators->head; gen != NULL; gen = gen->next) {
		assert(gen->g >= 0 && gen->g < g->n);
		col_of[gen->g] = col++;
	}
	assert(col == t);

	int i = 0;
	for (node_data gen = gd->generators->head; gen != NULL; gen = gen->next) {
		int curr_elt = fast_exp(g, gen->g, neutral, gen->p);
		data_list decomp = gd->decomps[curr_elt]; 

		trig_rel->t[i][i] -= gen->p;

		for (node_data c = decomp->head; c != NULL; c = c->next) {
			assert(c->g >= 0 && c->g < g->n);
			int j = col_of[c->g];
			assert(j >= 0 && j < t);
			trig_rel->t[i][j] += c->p;
		}
		i++;
	}

	free(col_of);
	return trig_rel;
}

basis create_empty_basis(int n) {
	basis b = malloc(sizeof(basis_s));
	b->n = n;
	b->elts = calloc(n, sizeof(int));
	b->order = calloc(n, sizeof(int));
	return b;
}
void free_basis(basis b) {
	free(b->elts);
	free(b->order);
	free(b);
}

void check_basis(group g, basis b, int neutral) {
	assert(is_neutral(g, neutral));
	for (int i = 0; i < b->n; i++) {
		int val = fast_exp(g, b->elts[i], neutral, b->order[i]);
		assert(val == neutral);
	}
}

void show_basis(basis b) {
	printf("Order:\n");
	print_arr(b->order, b->n);
	printf("Elements:\n");
	print_arr(b->elts, b->n);
}

basis get_basis_from_data(group g, group_data gd, int neutral, matrix D, matrix V_inv) {
	int t = gd->generators->length;
	printf("Generators: ");
	show_values_data_list(gd->generators);
	printf("\n");
	basis b = create_empty_basis(t);
	for (int i = 0; i < t; i++) {
		b->elts[i] = neutral;
		int j = 0;
		for (node_data gen = gd->generators->head; gen != NULL; gen=gen->next) {
			int to_add = fast_exp(g, gen->g, neutral, ((V_inv->t[i][j]) % g->n) + g->n);
			b->elts[i] = calculate(g, b->elts[i], to_add);
			j++;
		}
		b->order[i] = D->t[i][i];
	}

	return b;
}

basis get_basis_from_rel(group g, group_data gd, int neutral) {
	int t = gd->generators->length;
	matrix R = get_trig_rel_table(g, neutral, gd);
	matrix U = identiy(t);
	matrix V = identiy(t);
	matrix D = mat_alloc(t);
	mat_snf_with_diagonal(R, U, V, D);
	// mat_print("U", U);
	// mat_print("V", V);
	// mat_print("D", D);

	matrix V_inv = mat_inverse(V);
	// mat_print("V inverse", V_inv);
	mat_free(R);
	mat_free(U);
	mat_free(V);

	basis b = get_basis_from_data(g, gd, neutral, D, V_inv);
	mat_free(D);
	mat_free(V_inv);

	return b;
}

basis get_basis(group g) {
	int neutral = find_neutral(g);
	printf("[*] En train de calculer les données du groupe.\n");
	group_data gd = calculate_group_data(g);
	basis b = get_basis_from_rel(g, gd, neutral);
	free_group_data(gd);
	printf("Base: ");
	print_arr(b->elts, b->n);

	printf("Ordres: ");
	print_arr(b->order, b->n);

	return b;
}

bool are_isomorphic(group g1, group g2) {
	int neutral1 = find_neutral(g1);
	group_data gd1 = calculate_group_data(g1);
	matrix R1 = get_trig_rel_table(g1, neutral1, gd1);
	free_group_data(gd1);

	matrix U1 = identity(R1->n);
	matrix V1 = identity(R1->n);
	matrix D1 = mat_alloc(R1->n);

	mat_snf_with_diagonal(R1, U1, V1, D1);
	
	mat_free(R1);
	mat_free(U1);
	mat_free(V1);

	int neutral2 = find_neutral(g2);
	group_data gd2 = calculate_group_data(g2);
	matrix R2 = get_trig_rel_table(g2, neutral2, gd2);
	free_group_data(gd2);

	matrix U2 = identity(R2->n);
	matrix V2 = identity(R2->n);
	matrix D2 = mat_alloc(R2->n);

	mat_snf_with_diagonal(R2, U2, V2, D2);
	
	mat_free(R2);
	mat_free(U2);
	mat_free(V2);

	bool res = same_abelian_group_from_snf(D1, D2);

	mat_free(D1);
	mat_free(D2);

	return res;
}

void iterate_decomps(int* decomps, int dim, int i, int* orders) {
	if (i >= dim) {
		return;
	} else {
		if ((decomps[i] + 1) % orders[i] == 0) {
			decomps[i] = 0;
			iterate_decomps(decomps, dim, i+1, orders);
		} else {
			decomps[i] += 1;
		}
	}
}


int* calculate_isomorphism(group g1, group g2) {
	int neutral1 = find_neutral(g1);
	group_data gd1 = calculate_group_data(g1);
	basis b1 = get_basis_from_rel(g1, gd1, neutral1);

	int neutral2 = find_neutral(g2);
	group_data gd2 = calculate_group_data(g2);
	basis b2 = get_basis_from_rel(g2, gd2, neutral2);

	int* iso = malloc(g1->n*sizeof(int));
	for (int i = 0; i < g1->n; i++) {
		iso[i] = -1;
	}
	int i = b1->n - 1;
	int j = b2->n - 1;
	int dim = 0;

	while (b1->elts[i] != neutral1 && i >= 0 && b2->elts[j] != neutral2 && j >= 0) {
		iso[b1->elts[i]] = b2->elts[j];
		i--;
		j--;
		dim++;
	}

	int* real_basis_g1 = calloc(dim, sizeof(int));
	int* real_basis_g2 = calloc(dim, sizeof(int));
	int* orders = calloc(dim, sizeof(int));

	for (int i = 0; i < dim; i++) {
		real_basis_g1[i] = b1->elts[b1->n-1 - i];
		real_basis_g2[i] = b2->elts[b2->n-1 - i];
		orders[i] = b1->order[b1->n-1 - i];
	}

	int* decomps = calloc(dim, sizeof(int));

	for (int k = 0; k < g1->n; k++) {
		int curr_elt_g1 = neutral1;
		int curr_elt_g2 = neutral2;
		for (int i = 0; i < dim; i++) {
			int vec1 = real_basis_g1[i];
			int vec2 = real_basis_g2[i];
			curr_elt_g1 = calculate(g1, curr_elt_g1, fast_exp(g1, vec1, neutral1, decomps[i]));
			curr_elt_g2 = calculate(g2, curr_elt_g2, fast_exp(g2, vec2, neutral2, decomps[i]));
		}
		iso[curr_elt_g1] = curr_elt_g2;
		
		iterate_decomps(decomps, dim, 0, orders);
	}

	free(decomps);
	free(real_basis_g1);
	free(real_basis_g2);
	free(orders);

	free_group_data(gd1);
	free_group_data(gd2);

	free_basis(b1);
	free_basis(b2);

	return iso;
}

bool check_isomorphism(group g1, group g2, int* iso) {
	assert(g1->n == g2->n);
	int n = g1->n;
	for (int x = 0; x < n; x++) {
		for (int y = 0; y < n; y++) {
			int e_g1 = iso[calculate(g1, x, y)];
			int e_g2 = calculate(g2, iso[x], iso[y]);
			bool check = (e_g1 == e_g2);
			if (!check) {
				printf("[-] For (%d, %d): %d != %d.\n", x, y, e_g1, e_g2);
				return false;
			}
		}
	}

	return true;
}