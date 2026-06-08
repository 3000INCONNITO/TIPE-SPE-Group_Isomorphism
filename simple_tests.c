#include "Groups/group.h"
#include "Algorithms/algs.h"
#include <time.h>

static double elapsed(struct timespec start, struct timespec end) {
	return (end.tv_sec - start.tv_sec)
		+ (end.tv_nsec - start.tv_nsec) / 1e9;
}


void print_array(int* arr, int n) {
	for (int i = 0; i < n; i++) {
		printf("%d ", arr[i]);
	}
	printf("\n");
}

/*
4849845
37182145

*/

void test() {
	int ivf[] = {2,5,7,11,13,17,19,23};
	int length = sizeof(ivf) / sizeof(int);
	int n = 1;
	for (int i = 0; i < length; i++) {
		n*=ivf[i];
	}

	printf("Order: %d\nRank: %d\n", n, length);
	cpr_group my_group = create_cpr_group(n, length, ivf);

	{
		struct timespec start, end;
		gbg_collector trasher = create_empty_gbg_collector();
		clock_gettime(CLOCK_MONOTONIC, &start);
		group_data gd = calculate_group_data_fast_cpr(my_group, trasher);
		clock_gettime(CLOCK_MONOTONIC, &end);

		printf("[+] Done, time taken: %.9f.\n", elapsed(start, end));
		free_gbc_gdata(gd, trasher);
	}


	{
		struct timespec start, end;
		clock_gettime(CLOCK_MONOTONIC, &start);
		group_data gd = calculate_group_data_cpr(my_group);
		clock_gettime(CLOCK_MONOTONIC, &end);

		printf("[+] Done, time taken: %.9f.\n", elapsed(start, end));
		free_group_data(gd);
	}

	cpr_group_free(my_group);
}


int main() {
	test();

	return 0;
}

