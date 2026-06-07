#include "Garbage_Collector/garbage_collector.h"
#include "Groups/abelian_groups.h"
#include "Algorithms/algs.h"
#include "Algorithms/jules.h"
#include <time.h>
#include <stdio.h>

#define TEST_ORDER 20000

group create_group() {
	group g = create_abelian_group(TEST_ORDER);
	return g;
}

double time_calc_fast(int runs) {
	double total = 0.0;

	for (int i = 0; i < runs; i++) {
		group g = create_group();
		gbg_collector gbc = create_empty_gbg_collector();

		struct timespec start, end;

		clock_gettime(CLOCK_MONOTONIC, &start);

		group_data gd = calculate_group_data_fast(g, gbc);

		clock_gettime(CLOCK_MONOTONIC, &end);

		total += (end.tv_sec - start.tv_sec)
			+ (end.tv_nsec - start.tv_nsec) / 1e9;

		free_gbc_gdata(gd, gbc);
		group_free(g);
	}

	return total/runs;
}

double time_calc(int runs) {
	double total = 0.0;

	for (int i = 0; i < runs; i++) {
		group g = create_group();

		struct timespec start, end;

		clock_gettime(CLOCK_MONOTONIC, &start);

		group_data gd = calculate_group_data(g);

		clock_gettime(CLOCK_MONOTONIC, &end);

		total += (end.tv_sec - start.tv_sec)
			+ (end.tv_nsec - start.tv_nsec) / 1e9;

		free_group_data(gd);
		group_free(g);
	}

	return total/runs;
}

int main() {
	int runs = 10;

	double fast_time = time_calc_fast(runs);
	double normal_time = time_calc(runs);

	printf("Order: %d\n", TEST_ORDER);
	printf("Runs: %d\n\n", runs);

	printf("calculate_group_data_fast : %.9f s total (%.9e s/run)\n",
		fast_time, fast_time / runs);

	printf("calculate_group_data      : %.9f s total (%.9e s/run)\n",
		normal_time, normal_time / runs);

	printf("\nSpeedup: %.2fx\n",
		normal_time / fast_time);

	return 0;
}