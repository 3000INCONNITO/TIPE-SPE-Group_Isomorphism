


#ifndef SMITH_NORMAL_FORM_H
#define SMITH_NORMAL_FORM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef long long ll;

typedef struct {
	int  n;
	ll **t;
} matrix_s;

typedef matrix_s *matrix;

matrix mat_alloc(int n);
void set_matrix(int n, matrix M, int m[][n]);
matrix	from_given_matrix(int n, int m[][n]);
matrix identiy(int n);
matrix identity(int n);
void mat_free(matrix A);

void mat_identity(matrix A);
void mat_print(const char *name, matrix A);

void smith_normal_form(matrix M, matrix U, matrix V);

void mat_mul(matrix C, matrix A, matrix B);
matrix get_prod(matrix A, matrix B);
bool mat_equal(matrix A, matrix B);
bool same_abelian_group_from_snf(matrix D1, matrix D2);

int mat_inverse_bareiss(matrix M, matrix adj, ll *det_out);
matrix mat_inverse(matrix M);
matrix mat_snf_V_inv(matrix R);
void mat_snf_with_diagonal(matrix M, matrix U, matrix V, matrix D);
#endif