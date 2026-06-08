#include "Groups/group.h"
#include "Algorithms/algs.h"

void print_array(int* arr, int n) {
	for (int i = 0; i < n; i++) {
		printf("%d ", arr[i]);
	}
	printf("\n");
}

void test() {
	int ivf[] = {3, 5, 7, 11, 13};
	int length = sizeof(ivf) / sizeof(int);
	int n = 1;
	for (int i = 0; i < length; i++) {
		n*=ivf[i];
	}


	printf("Order: %d\nRank: %d\n", n, length);
	cpr_group my_group = create_cpr_group(n, length, ivf);
	gbg_collector trasher = create_empty_gbg_collector();

	group_data gd = calculate_group_data_fast_cpr(my_group, trasher);
	//show_rel(gd);

	printf("[+] Done.\n");


	free_gbc_gdata(gd, trasher);
	cpr_group_free(my_group);
}


int main() {
	test();

	return 0;
}

