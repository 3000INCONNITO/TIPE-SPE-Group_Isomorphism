// #include "Groups/group.h"
#include "Groups/example_groups.h"
#include "Groups/abelian_groups.h"
// #include "Gen_Table/gen_table.h"
#include "Algorithms/algs.h"

#include <time.h>

/* Multiply two group elements. */
int group_mul(int i, int j) { return cayley[i][j]; }

void fisher_yates_shuffle(int *array, int n) {
	for (int i = n - 1; i > 0; i--) {
		int j = rand() % (i + 1); // j est un indice aléatoire entre 0 et i
		int temp = array[i];
		array[i] = array[j];
		array[j] = temp;
	}
}

int* inverse(int* p, int n) {
	int* inv = malloc(n*sizeof(int));
	for (int i = 0; i < n; i++) {
		inv[p[i]] = i;
	}
	return inv;
}

int* random_permutation(int n) {
	int* arr = malloc(n*sizeof(int));
	for (int i = 0; i < n; i++) {
		arr[i] = i;
	}
	fisher_yates_shuffle(arr, n);

	return arr;
}

/* Compute the order of element i. */
static int element_order(int i) {
    int cur = 0;
    for (int k = 1; k <= ORDER; k++) {
        cur = cayley[cur][i];
        if (cur == 0) return k;
    }
    return -1; /* should never happen */
}

/* Sanity checks. */
static void verify(void) {
    int seen[ORDER];

    /* Identity */
    for (int i = 0; i < ORDER; i++) {
        assert(cayley[0][i] == i);
        assert(cayley[i][0] == i);
    }

    /* Latin square (every row and column is a permutation) */
    for (int i = 0; i < ORDER; i++) {
        for (int k = 0; k < ORDER; k++) seen[k] = 0;
        for (int j = 0; j < ORDER; j++) seen[cayley[i][j]]++;
        for (int k = 0; k < ORDER; k++) assert(seen[k] == 1);
    }

    /* Associativity */
    for (int i = 0; i < ORDER; i++)
        for (int j = 0; j < ORDER; j++)
            for (int k = 0; k < ORDER; k++)
                assert(cayley[cayley[i][j]][k] == cayley[i][cayley[j][k]]);

    /* Commutativity (G is abelian) */
    for (int i = 0; i < ORDER; i++)
        for (int j = 0; j < ORDER; j++)
            assert(cayley[i][j] == cayley[j][i]);

    /* Non-cyclicity: max element order must be < 60 */
    int max_ord = 0;
    for (int i = 0; i < ORDER; i++) {
        int o = element_order(i);
        if (o > max_ord) max_ord = o;
    }
    assert(max_ord < ORDER); /* would be 60 if cyclic */
    printf("Max element order: %d (< %d => NOT cyclic)\n", max_ord, ORDER);

    printf("All group axioms verified. G is abelian, order 60, non-cyclic.\n");
}

/* Pretty-print the table. */
static void print_table(void) {
    printf("\nCayley table for Z/2Z x Z/2Z x Z/3Z x Z/5Z  (order 60)\n");
    printf("      |");
    for (int j = 0; j < ORDER; j++) printf(" %2d", j);
    printf("\n------+");
    for (int j = 0; j < ORDER; j++) printf("---");
    printf("\n");
    for (int i = 0; i < ORDER; i++) {
        printf("  %2d  |", i);
        for (int j = 0; j < ORDER; j++) printf(" %2d", cayley[i][j]);
        printf("\n");
    }
}

void print_order(int* arr, int n) {
	for (int i = 0; i < n; i++) {
		printf("(%d: %d) ", i, arr[i]);
	}
	printf("\n");
}

void test_D3() {
	int n = 6;
	group g = group_empty(n);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			g->table[i][j] = d3[i][j];
		}
	}
	group_name(g, "D3");
	group_print(g);

	int e = find_neutral(g);
	printf("Neutral = %d ?\n", e);
	for (int i = 0; i < g->n; i++) {
		printf("%d * %d = %d\n", e, i, calculate(g, e, i));
	}

	basis b = get_basis(g);
	
	free_basis(b);

	group_free(g);
}

void test_D4() {
	int n = 8;
	group g = group_empty(n);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			g->table[i][j] = d4[i][j];
		}
	}
	group_name(g, "D4");
	group_print(g);

	int e = find_neutral(g);
	printf("Neutre = %d\n", e);
	for (int i = 0; i < g->n; i++) {
		printf("%d * %d = %d\n", e, i, calculate(g, e, i));
	}

	basis b = get_basis(g);
	
	free_basis(b);

	group_free(g);
}

void test_fast_exp() {
	int n = 16;
	group g = group_empty(n);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			g->table[i][j] = z16_table[i][j];
		}
	}
	group_name(g, "Z/16Z");

	int e = find_neutral(g);
	printf("Neutral: %d\n", e);

	for (int i = 0; i < n; i++) {
		printf("%d \n", fast_exp(g, 2, e, i));
	}


	group_free(g);
}

void test_calculate_subgroup() {
	int n = 8;
	group g = group_empty(n);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			g->table[i][j] = Z4ZxZ2Z[i][j];
		}
	}
	group_name(g, "Z/4Z x Z/2Z");
	group_print(g);

	int neutral_elt = get_neutral(g);
	printf("Elément neutre: %d\n", neutral_elt);

	group_data gd = create_empty_group_data(n);

	int gen;

	printf("\n");

	gen = calculate_next_gen_and_rel(g, gd, 1, neutral_elt);
	show_rel(gd);
	show_generators(gd);
	show_num_elts(gd);
	show_grp_elts(gd);
	printf("\n");

	gen = calculate_next_gen_and_rel(g, gd, 3, neutral_elt);
	show_rel(gd);
	show_generators(gd);
	show_num_elts(gd);
	show_grp_elts(gd);
	printf("\n");

	// gen = calculate_next_gen_and_rel(g, gd, 4, neutral_elt);
	// show_rel(gd);
	// show_generators(gd);
	// show_num_elts(gd);
	// show_grp_elts(gd);
	// printf("\n");

	free_group_data(gd);
	group_free(g);
}

void test_calculate_subgroup_large() {
	int n = 101;
	group g = group_empty(n);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			g->table[i][j] = (i + j) % 101;
		}
	}
	group_name(g, "Z/nZ");
	group_print(g);

	int neutral_elt = get_neutral(g);
	printf("Elément neutre: %d\n", neutral_elt);

	group_data gd = create_empty_group_data(n);

	int gen;

	printf("\n");

	gen = calculate_next_gen_and_rel(g, gd, 1, neutral_elt);
	show_rel(gd);
	show_generators(gd);
	show_num_elts(gd);
	show_grp_elts(gd);
	printf("\n");

	gen = calculate_next_gen_and_rel(g, gd, 3, neutral_elt);
	show_rel(gd);
	show_generators(gd);
	show_num_elts(gd);
	show_grp_elts(gd);
	printf("\n");

	// gen = calculate_next_gen_and_rel(g, gd, 4, neutral_elt);
	// show_rel(gd);
	// show_generators(gd);
	// show_num_elts(gd);
	// show_grp_elts(gd);
	// printf("\n");

	free_group_data(gd);
	group_free(g);
}

void test_calculate_group_data() {
	int n = 60;
	group g1 = group_empty(n);
	group g2 = group_empty(n);

	int* iso = malloc(n*sizeof(int));
	for(int i = 0; i < n; i++) {
		iso[i] = i;
	}
	fisher_yates_shuffle(iso, n);
	int* inv = inverse(iso, n);

	print_arr(iso, n);

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			g1->table[i][j] = cayley[i][j];
			g2->table[i][j] = iso[ cayley[ inv[i] ][ inv[j] ] ];
		}
	}
	group_name(g1, "Z/2Z x Z/30Z");
	group_name(g2, "Z/2Z x Z/30Z - iso");

	// Group 1
	basis b1 = get_basis(g1);
	int e1 = find_neutral(g1);
	printf("Neutral g1: %d\n", e1);
	for (int i = 0; i < b1->n; i++) {
		printf("%d\n", fast_exp(g1, b1->elts[i], e1, b1->order[i]));
	}
	free_basis(b1);
	group_free(g1);

	// Group 2
	basis b2 = get_basis(g2);
	int e2 = find_neutral(g2);
	printf("Neutral g2: %d\n", e2);
	for (int i = 0; i < b2->n; i++) {
		printf("%d\n", fast_exp(g2, b2->elts[i], e2, b2->order[i]));
	}
	free_basis(b2);
	group_free(g2);

	free(inv);
	free(iso);
}

void general_test() {
	int n = 60;
	// int iso[60] = {
	// 	1, 0, 2, 3, 4, 5, 6, 7, 8, 9,
	// 	10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
	// 	20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
	// 	30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
	// 	40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
	// 	50, 51, 52, 53, 54, 55, 56, 57, 58, 59
	// };
	int* iso = malloc(n*sizeof(int));
	for(int i = 0; i < n; i++) {
		iso[i] = i;
	}
	fisher_yates_shuffle(iso, n);
	//print_arr(iso, n);
	int* inv = inverse(iso, n);

	group g = group_empty(n);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			g->table[i][j] = iso[cayley [inv[i]][inv[j]] ];
		}
	}
	int neutral = find_neutral(g);
	printf("Neutral: %d\n", neutral);

	group_data gd = calculate_group_data(g);
	show_generators(gd);
	//show_rel(gd);
	check_group_data(g, gd, neutral);
	printf("[+] Passed test.\n");


	matrix R = get_trig_rel_table(g, neutral, gd);
	mat_print("Trig relations", R);

	matrix U = identiy(R->n);
	matrix V = identiy(R->n);
	matrix D = mat_alloc(R->n);
	matrix Id = identiy(R->n);
	mat_print("Identity", Id);
	mat_snf_with_diagonal(R, U, V, D);

	matrix A = get_prod(U, R);
	matrix B = get_prod(A, V);


	mat_print("URV", B);
	mat_free(A);
	mat_free(B);

	matrix V_inv = mat_inverse(V);
	matrix prod = get_prod(V, V_inv);
	mat_print("V x V_inv", prod);
	assert(mat_equal(prod, Id));
	mat_free(prod);


	basis b = get_basis_from_data(g, gd, neutral, D, V_inv);
	printf("Neutral: %d\n", neutral);
	print_arr(b->elts, b->n);
	print_arr(b->order, b->n);

	check_basis(g, b, neutral);
	printf("[+] Passed basis test.\n");


	free_basis(b);
	mat_free(Id);
	mat_free(R);
	mat_free(U);
	mat_free(V);
	mat_free(D);
	mat_free(V_inv);

	free_group_data(gd);
	group_free(g);
	free(iso);
	free(inv);
}

void test_isomorphic() {
	int n = 101; //60;
	group g1 = group_empty(n);
	group g2 = group_empty(n);

	int* iso = malloc(n*sizeof(int));
	for(int i = 0; i < n; i++) {
		iso[i] = i;
	}
	fisher_yates_shuffle(iso, n);
	int* inv = inverse(iso, n);

	// print_arr(iso, n);

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			g1->table[i][j] = (i+j) % n; //cayley[i][j];
			g2->table[i][j] = iso[ (inv[i] + inv[j]) % n ];//iso[ cayley[ inv[i] ][ inv[j] ] ];
		}
	}
	group_name(g1, "G");
	group_name(g2, "G - iso");

	int* res = calculate_isomorphism(g1, g2);

	printf("Check isomorphism: %d\n", check_isomorphism(g1, g2, res));

	group_free(g1);
	group_free(g2);

	free(inv);
	free(iso);

	free(res);
}

int main() {
	srand(time(NULL));
	//test_D4();
	//test_calculate_subgroup();
	//test_fast_exp();

	// verify();
    // print_table();


	//test_calculate_group_data();
	//general_test();

	test_isomorphic();

	//test_calculate_subgroup_large();

	return 0;
}