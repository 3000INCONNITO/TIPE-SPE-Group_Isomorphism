
#include "Algorithms/snf.h"



int main() {
	printf("STARTING TESTS.\n");

	int test_matrix[4][4] = {
		{-2, 0, 0, 5},
		{0, -2, 0, 5},
		{0, 0, -3, 5},
		{0, 0, 0, -5}
	};

	matrix R = from_given_matrix(4, test_matrix);
	matrix U = identiy(4);
	matrix V = identiy(4);

	printf("Before:\n");
	mat_print("R", R);
	mat_print("U", U);
	mat_print("V", V);

	smith_normal_form(R, U, V);

	printf("After:\n");
	mat_print("R", R);
	mat_print("U", U);
	mat_print("V", V);

	printf("Inverse of V:\n");
	matrix V_inv = mat_inverse(V);
	mat_print("V inverse", V_inv);

	/*
	
[   -2    0    0    5 ]
[   20   -2   -3  -40 ]
[  -25    3    3   50 ]
[    8   -1   -1  -16 ]
	
	*/


	mat_free(R);
	mat_free(U);
	mat_free(V);
	mat_free(V_inv);
	


	return 0;
}
