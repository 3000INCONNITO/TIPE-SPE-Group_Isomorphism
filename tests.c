#include "Garbage_Collector/garbage_collector.h"
#include "Groups/abelian_groups.h"
#include "Algorithms/algs.h"
#include "Algorithms/jules.h"
#include <time.h>
#include <stdio.h>

#define MAX_ORDER 15000
#define RUNS 15

static double elapsed(struct timespec start, struct timespec end) {
	return (end.tv_sec - start.tv_sec)
		+ (end.tv_nsec - start.tv_nsec) / 1e9;
}

int main(void) {

	FILE *f = fopen("benchmark.txt", "w");
	if (f == NULL) {
		perror("fopen");
		return 1;
	}

	for (int order = 10728 + 1; order <= MAX_ORDER; order += 50) {

		printf("Order %d/%d\n", order, MAX_ORDER);

		double fast_total = 0.0;
		double normal_total = 0.0;

		group g = create_abelian_group(order);

		for (int run = 0; run < RUNS; run++) {

			/* Fast version */
			{
				gbg_collector gbc = create_empty_gbg_collector();

				struct timespec start, end;

				clock_gettime(CLOCK_MONOTONIC, &start);
				group_data gd = calculate_group_data_fast(g, gbc);
				clock_gettime(CLOCK_MONOTONIC, &end);

				fast_total += elapsed(start, end);

				free_gbc_gdata(gd, gbc);
			}

			/* Normal version */
			{
				struct timespec start, end;

				clock_gettime(CLOCK_MONOTONIC, &start);
				group_data gd = calculate_group_data(g);
				clock_gettime(CLOCK_MONOTONIC, &end);

				normal_total += elapsed(start, end);

				free_group_data(gd);
			}
		}

		group_free(g);

		double fast_avg = fast_total / RUNS;
		double normal_avg = normal_total / RUNS;

		fprintf(
			f,
			"Order: %d\n"
			"Fast: %.9f\n"
			"Normal: %.9f\n"
			"Speedup: %.4f\n"
			"\n",
			order,
			fast_avg,
			normal_avg,
			normal_avg / fast_avg
		);

		fflush(f);
	}

	fclose(f);

	return 0;
}