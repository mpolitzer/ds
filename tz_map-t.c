#include <stdio.h>

#define TZ_UNIT_TEST_INLINE
#include "tz_unit_test.h"

#define TZ_DS_MAP_INLINE
#include "tz_map.h"

uint32_t int_hash(tz_map *me, uint64_t u)
{
	return u;
}

bool int_eq(tz_map *me, uint64_t u, uint64_t v)
{
	return u == v;
}

void test_stress()
{
	unsigned int seed = 1;

	tz_map hm = tz_map_create(int_hash, int_eq);

	uint64_t err=0, n=100000;

	srand(seed);
	for (uint64_t i=0; i<n; ++i) {
		int r = rand();
		tz_map_put(&hm, r, r);
	}

	srand(seed);
	for (uint64_t i=0; i<n; ++i) {
		int r = rand();
		uint64_t *val;
		if ((val = tz_map_get(&hm, r)) && *val != r) {
			err++;
		}
	}

	tz_unit_test_should_pass(err == 0);
}

int main(int argc, char const* argv[])
{
	tz_unit_test_start(argv[0]);
	test_stress();
	return tz_unit_test_report();
}
