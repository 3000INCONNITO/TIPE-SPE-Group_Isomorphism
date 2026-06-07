#include "Garbage_Collector/garbage_collector.h"
#include "Groups/abelian_groups.h"
#include "Algorithms/algs.h"
#include "Algorithms/jules.h"
#include <time.h>
#include <stdio.h>

#define TEST_ORDER 30030

group create_group() {
	group g = create_abelian_group(TEST_ORDER);
	return g;
}

int main() {
	int runs = 20;

	double fast_total = 0.0;
	double normal_total = 0.0;

	for (int i = 0; i < runs; i++) {
		group g = create_group();

		/* Fast version */
		{
			gbg_collector gbc = create_empty_gbg_collector();

			struct timespec start, end;

			clock_gettime(CLOCK_MONOTONIC, &start);

			group_data gd = calculate_group_data_fast(g, gbc);

			clock_gettime(CLOCK_MONOTONIC, &end);

			fast_total +=
				(end.tv_sec - start.tv_sec)
				+ (end.tv_nsec - start.tv_nsec) / 1e9;

			free_gbc_gdata(gd, gbc);
		}

		/* Normal version */
		{
			struct timespec start, end;

			clock_gettime(CLOCK_MONOTONIC, &start);

			group_data gd = calculate_group_data(g);

			clock_gettime(CLOCK_MONOTONIC, &end);

			normal_total +=
				(end.tv_sec - start.tv_sec)
				+ (end.tv_nsec - start.tv_nsec) / 1e9;

			free_group_data(gd);
		}

		group_free(g);
	}

	double fast_time = fast_total / runs;
	double normal_time = normal_total / runs;

	printf("Order: %d\n", TEST_ORDER);
	printf("Runs: %d\n\n", runs);

	printf("calculate_group_data_fast : %.9f s\n", fast_time);
	printf("calculate_group_data      : %.9f s\n", normal_time);

	printf("\nSpeedup: %.2fx\n",
		normal_time / fast_time);

	return 0;
}