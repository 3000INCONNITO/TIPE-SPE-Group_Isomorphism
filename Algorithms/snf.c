


#include "snf.h"

static ll extended_gcd(ll a, ll b, ll *x, ll *y) {
	if (b == 0) {
		*x = 1;
		*y = 0;
		return a;
	}
	ll x1, y1, g;

	g  = extended_gcd(b, a % b, &x1, &y1);
	*x = y1;
	*y = x1 - (a / b) * y1;
	return g;
}

matrix mat_alloc(int n) {
	if (n <= 0)
		return NULL;

	matrix A = (matrix)malloc(sizeof(matrix_s));
	if (A == NULL)
		return NULL;

	A->n = n;
	A->t = (ll **)calloc((size_t)n, sizeof(ll *));
	if (A->t == NULL) {
		free(A);
		return NULL;
	}

	int i = 0;
	while (i < n) {
		A->t[i] = (ll *)calloc((size_t)n, sizeof(ll));
		if (A->t[i] == NULL) {
			while (--i >= 0)
				free(A->t[i]);
			free(A->t);
			free(A);
			return NULL;
		}
		i++;
	}
	return A;
}

void set_matrix(int n, matrix M, int m[][n]) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			M->t[i][j] = m[i][j];
		}
	}
}

matrix from_given_matrix(int n, int m[][n]) {
	matrix M = mat_alloc(n);
	set_matrix(n, M, m);
	return M;
}

matrix identiy(int n) {
	matrix res = mat_alloc(n);
	for(int i = 0; i < n; i++) {
		res->t[i][i] = 1;
	}
	return res;
}

matrix identity(int n) {
	return identiy(n);
}

void mat_free(matrix A) {
	if (A == NULL)
		return;
	int i = 0;
	while (i < A->n) {
		free(A->t[i]);
		i++;
	}
	free(A->t);
	free(A);
}

void mat_identity(matrix A) {
	int i = 0;
	while (i < A->n) {
		int j = 0;
		while (j < A->n) {
			A->t[i][j] = (i == j) ? 1 : 0;
			j++;
		}
		i++;
	}
}

void mat_print(const char *name, matrix A) {
	int i, j;

	printf("%s (%dx%d):\n", name, A->n, A->n);
	i = 0;
	while (i < A->n) {
		printf("  [");
		j = 0;
		while (j < A->n) {
			printf(" %4lld", A->t[i][j]);
			j++;
		}
		printf(" ]\n");
		i++;
	}
}

static void swap_rows(matrix A, matrix U, int i, int j) {
	int k;

	if (i == j)
		return;
	k = 0;
	while (k < A->n) {
		ll t       = A->t[i][k];
		A->t[i][k] = A->t[j][k];
		A->t[j][k] = t;
		k++;
	}
	if (U) {
		k = 0;
		while (k < U->n) {
			ll t       = U->t[i][k];
			U->t[i][k] = U->t[j][k];
			U->t[j][k] = t;
			k++;
		}
	}
}

static void swap_cols(matrix A, matrix V, int i, int j) {
	int k;

	if (i == j)
		return;
	k = 0;
	while (k < A->n)
	{
		ll t        = A->t[k][i];
		A->t[k][i]  = A->t[k][j];
		A->t[k][j]  = t;
		k++;
	}
	if (V)
	{
		k = 0;
		while (k < V->n)
		{
			ll t        = V->t[k][i];
			V->t[k][i]  = V->t[k][j];
			V->t[k][j]  = t;
			k++;
		}
	}
}

static void row_combine(matrix A, matrix U, int r0, int r1, ll a, ll b, ll c, ll d) {
	int k = 0;
	while (k < A->n) {
		ll v0 = A->t[r0][k];
		ll v1 = A->t[r1][k];

		A->t[r0][k] = a * v0 + b * v1;
		A->t[r1][k] = c * v0 + d * v1;
		k++;
	}
	if (U) {
		k = 0;
		while (k < U->n) {
			ll v0 = U->t[r0][k];
			ll v1 = U->t[r1][k];

			U->t[r0][k] = a * v0 + b * v1;
			U->t[r1][k] = c * v0 + d * v1;
			k++;
		}
	}
}

static ll ll_abs_snf(ll x) {
	return (x < 0) ? -x : x;
}

static void add_row_multiple(matrix A, matrix U, int dst, int src, ll k) {
	if (k == 0 || dst == src)
		return;
	for (int j = 0; j < A->n; j++)
		A->t[dst][j] += k * A->t[src][j];
	if (U) {
		for (int j = 0; j < U->n; j++)
			U->t[dst][j] += k * U->t[src][j];
	}
}

static void add_col_multiple(matrix A, matrix V, int dst, int src, ll k) {
	if (k == 0 || dst == src)
		return;
	for (int i = 0; i < A->n; i++)
		A->t[i][dst] += k * A->t[i][src];
	if (V) {
		for (int i = 0; i < V->n; i++)
			V->t[i][dst] += k * V->t[i][src];
	}
}

static void negate_row(matrix A, matrix U, int row) {
	for (int j = 0; j < A->n; j++)
		A->t[row][j] = -A->t[row][j];
	if (U) {
		for (int j = 0; j < U->n; j++)
			U->t[row][j] = -U->t[row][j];
	}
}

static int has_offdiag_in_pivot_cross(matrix M, int p) {
	for (int i = p + 1; i < M->n; i++)
		if (M->t[i][p] != 0 || M->t[p][i] != 0)
			return 1;
	return 0;
}

static void reduce_pivot_cross(matrix M, matrix U, matrix V, int p) {
	int n = M->n;

	while (has_offdiag_in_pivot_cross(M, p)) {
		for (int i = p + 1; i < n; i++) {
			while (M->t[i][p] != 0) {
				if (ll_abs_snf(M->t[i][p]) < ll_abs_snf(M->t[p][p]))
					swap_rows(M, U, p, i);
				ll q = M->t[i][p] / M->t[p][p];
				if (q == 0)
					q = (M->t[i][p] > 0) == (M->t[p][p] > 0) ? 1 : -1;
				add_row_multiple(M, U, i, p, -q);
			}
		}

		for (int j = p + 1; j < n; j++) {
			while (M->t[p][j] != 0) {
				if (ll_abs_snf(M->t[p][j]) < ll_abs_snf(M->t[p][p]))
					swap_cols(M, V, p, j);
				ll q = M->t[p][j] / M->t[p][p];
				if (q == 0)
					q = (M->t[p][j] > 0) == (M->t[p][p] > 0) ? 1 : -1;
				add_col_multiple(M, V, j, p, -q);
			}
		}
	}
}

void smith_normal_form(matrix M, matrix U, matrix V) {
	int n = M->n;

	for (int p = 0; p < n; p++) {
		int found = 0;
		int bi = p;
		int bj = p;
		ll best = 0;

		for (int i = p; i < n; i++) {
			for (int j = p; j < n; j++) {
				ll av = ll_abs_snf(M->t[i][j]);
				if (av != 0 && (!found || av < best)) {
					found = 1;
					best = av;
					bi = i;
					bj = j;
				}
			}
		}

		if (!found)
			break;

		swap_rows(M, U, p, bi);
		swap_cols(M, V, p, bj);

		for (;;) {
			reduce_pivot_cross(M, U, V, p);

			ll piv = M->t[p][p];
			int bad_i = -1;
			for (int i = p + 1; i < n && bad_i < 0; i++) {
				for (int j = p + 1; j < n; j++) {
					if (M->t[i][j] % piv != 0) {
						bad_i = i;
						break;
					}
				}
			}

			if (bad_i < 0)
				break;
			add_row_multiple(M, U, p, bad_i, 1);
		}

		if (M->t[p][p] < 0)
			negate_row(M, U, p);
	}
}

void mat_mul(matrix C, matrix A, matrix B) {
	int n = A->n;
	int i, j, k;

	i = 0;
	while (i < n) {
		j = 0;
		while (j < n) {
			C->t[i][j] = 0;
			j++;
		}
		i++;
	}
	i = 0;
	while (i < n) {
		k = 0;
		while (k < n) {
			ll aik = A->t[i][k];
			if (aik != 0) {
				j = 0;
				while (j < n) {
					C->t[i][j] += aik * B->t[k][j];
					j++;
				}
			}
			k++;
		}
		i++;
	}
}

matrix get_prod(matrix A, matrix B) {
	matrix C = mat_alloc(A->n);
	mat_mul(C, A, B);
	return C;
}

bool mat_equal(matrix A, matrix B) {
	if (A->n != B->n) return false;
	int n = A->n;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (A->t[i][j] != B->t[i][j]) {
				return false;
			}
		}
	}
	return true;
}

static int det_bareiss_array(const ll *src, int n, ll *det_out) {
	if (n <= 0 || src == NULL || det_out == NULL)
		return -1;
	if (n == 1) {
		*det_out = src[0];
		return 0;
	}

	ll *a = (ll *)malloc((size_t)n * (size_t)n * sizeof(ll));
	if (a == NULL)
		return -1;
	memcpy(a, src, (size_t)n * (size_t)n * sizeof(ll));

	ll prev = 1;
	int sign = 1;

	for (int k = 0; k < n - 1; k++) {
		int pivot = k;
		ll best = ll_abs_snf(a[(size_t)k * (size_t)n + (size_t)k]);
		for (int i = k + 1; i < n; i++) {
			ll av = ll_abs_snf(a[(size_t)i * (size_t)n + (size_t)k]);
			if (av > best) {
				best = av;
				pivot = i;
			}
		}

		if (best == 0) {
			*det_out = 0;
			free(a);
			return 0;
		}

		if (pivot != k) {
			for (int j = 0; j < n; j++) {
				ll tmp = a[(size_t)k * (size_t)n + (size_t)j];
				a[(size_t)k * (size_t)n + (size_t)j] =
					a[(size_t)pivot * (size_t)n + (size_t)j];
				a[(size_t)pivot * (size_t)n + (size_t)j] = tmp;
			}
			sign = -sign;
		}

		ll piv = a[(size_t)k * (size_t)n + (size_t)k];
		for (int i = k + 1; i < n; i++) {
			for (int j = k + 1; j < n; j++) {
				ll val = piv * a[(size_t)i * (size_t)n + (size_t)j]
					- a[(size_t)i * (size_t)n + (size_t)k]
						* a[(size_t)k * (size_t)n + (size_t)j];
				a[(size_t)i * (size_t)n + (size_t)j] = val / prev;
			}
		}
		prev = piv;

		for (int i = k + 1; i < n; i++)
			a[(size_t)i * (size_t)n + (size_t)k] = 0;
	}

	*det_out = (ll)sign * a[(size_t)(n - 1) * (size_t)n + (size_t)(n - 1)];
	free(a);
	return 0;
}

static int matrix_det_bareiss(matrix M, ll *det_out) {
	int n = M->n;
	ll *buf = (ll *)malloc((size_t)n * (size_t)n * sizeof(ll));
	if (buf == NULL)
		return -1;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++)
			buf[(size_t)i * (size_t)n + (size_t)j] = M->t[i][j];
	}
	int rc = det_bareiss_array(buf, n, det_out);
	free(buf);
	return rc;
}

static int minor_det_bareiss(matrix M, int skip_row, int skip_col, ll *det_out) {
	int n = M->n;
	if (n == 1) {
		*det_out = 1;
		return 0;
	}

	int m = n - 1;
	ll *minor = (ll *)malloc((size_t)m * (size_t)m * sizeof(ll));
	if (minor == NULL)
		return -1;

	int rr = 0;
	for (int i = 0; i < n; i++) {
		if (i == skip_row)
			continue;
		int cc = 0;
		for (int j = 0; j < n; j++) {
			if (j == skip_col)
				continue;
			minor[(size_t)rr * (size_t)m + (size_t)cc] = M->t[i][j];
			cc++;
		}
		rr++;
	}

	int rc = det_bareiss_array(minor, m, det_out);
	free(minor);
	return rc;
}

int mat_inverse_bareiss(matrix M, matrix adj, ll *det_out) {
	if (M == NULL || adj == NULL || det_out == NULL || M->n <= 0 || adj->n != M->n)
		return -1;

	int n = M->n;
	if (matrix_det_bareiss(M, det_out) != 0)
		return -1;

	if (*det_out == 0) {
		for (int i = 0; i < n; i++)
			for (int j = 0; j < n; j++)
				adj->t[i][j] = 0;
		return -1;
	}

	if (n == 1) {
		adj->t[0][0] = 1;
		return 0;
	}

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			ll d = 0;
			if (minor_det_bareiss(M, i, j, &d) != 0)
				return -1;
			if (((i + j) & 1) != 0)
				d = -d;
			adj->t[j][i] = d;
		}
	}
	return 0;
}

matrix mat_inverse(matrix M) {
	if (M == NULL || M->n <= 0)
		return NULL;

	int n = M->n;
	matrix W = mat_alloc(n);
	matrix Inv = mat_alloc(n);
	if (W == NULL || Inv == NULL) {
		mat_free(W);
		mat_free(Inv);
		return NULL;
	}

	for (int i = 0; i < n; i++)
		memcpy(W->t[i], M->t[i], (size_t)n * sizeof(ll));
	mat_identity(Inv);

	for (int k = 0; k < n; k++) {
		int pivot_row = -1;
		for (int i = k; i < n; i++) 
		{
			if (W->t[i][k] != 0) {
				pivot_row = i;
				break;
			}
		}
		if (pivot_row == -1) {
			mat_free(W);
			mat_free(Inv);
			return NULL;
		}
		swap_rows(W, Inv, k, pivot_row);

		for (int i = k + 1; i < n; i++) {
			while (W->t[i][k] != 0) {
				ll pivot = W->t[k][k];
				ll val = W->t[i][k];
				ll x, y, g;

				ll sp = (pivot < 0) ? -1 : 1;
				ll sv = (val < 0) ? -1 : 1;
				ll ap = pivot * sp;
				ll av = val * sv;
				g = extended_gcd(ap, av, &x, &y);
				x *= sp;
				y *= sv;

				row_combine(W, Inv, k, i, x, y, -(val / g), pivot / g);
			}
		}

		if (W->t[k][k] != 1 && W->t[k][k] != -1) {
			mat_free(W);
			mat_free(Inv);
			return NULL;
		}

		if (W->t[k][k] == -1) {
			for (int j = 0; j < n; j++) {
				W->t[k][j] = -W->t[k][j];
				Inv->t[k][j] = -Inv->t[k][j];
			}
		}

		for (int i = 0; i < n; i++) {
			if (i == k || W->t[i][k] == 0)
				continue;
			ll factor = W->t[i][k];
			for (int j = 0; j < n; j++) {
				W->t[i][j] -= factor * W->t[k][j];
				Inv->t[i][j] -= factor * Inv->t[k][j];
			}
		}
	}

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			ll expected = (i == j) ? 1 : 0;
			if (W->t[i][j] != expected) {
				mat_free(W);
				mat_free(Inv);
				return NULL;
			}
		}
	}

	mat_free(W);
	return Inv;
}

matrix mat_snf_V_inv(matrix R) {
    int n = R->n;
    matrix U = mat_alloc(n);
    matrix V = mat_alloc(n);
    matrix M = mat_alloc(n);

    int i = 0;
    while (i < n) {
        memcpy(M->t[i], R->t[i], (size_t)n * sizeof(ll));
        i++;
    }

    mat_identity(U);
    mat_identity(V);
    smith_normal_form(M, U, V);

    matrix V_inv = mat_inverse(V);

    mat_free(U);
    mat_free(V);
    mat_free(M);
    return V_inv;
}

void mat_snf_with_diagonal(matrix M, matrix U, matrix V, matrix D) {
    int n = M->n;
    int i = 0;

    while (i < n) {
        memcpy(D->t[i], M->t[i], (size_t)n * sizeof(ll));
        i++;
    }

    mat_identity(U);
    mat_identity(V);
    smith_normal_form(D, U, V);
}

bool same_abelian_group_from_snf(matrix D1, matrix D2) {
    int i = 0;
    int j = 0;

    while (i < D1->n || j < D2->n) {
        while (i < D1->n && abs(D1->t[i][i]) == 1) i++;
        while (j < D2->n && abs(D2->t[j][j]) == 1) j++;

        if (i >= D1->n || j >= D2->n) {
            return i >= D1->n && j >= D2->n;
        }

        if (abs(D1->t[i][i]) != abs(D2->t[j][j])) {
            return false;
        }

        i++;
        j++;
    }

    return true;
}