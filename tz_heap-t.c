#include <limits.h>

#define TZ_UNIT_TEST_INLINE
#include "tz_unit_test.h"

#define TZ_DS_HEAP_INLINE
#include "tz_heap.h"

bool int_gt(tz_heap *h, void *u, void *v)
{
	return *(int *)u > *(int *)v;
}

bool int_lt(tz_heap *h, void *u, void *v)
{
	return *(int *)u < *(int *)v;
}

void test_smoke()
{
	{
		tz_heap h = tz_heap_create(sizeof(int), int_lt);

		tz_heap_put(&h, &(int){0});
		tz_heap_put(&h, &(int){1});
		tz_heap_put(&h, &(int){2});
		tz_heap_put(&h, &(int){3});

		bool eq = true;
		int prv = INT_MAX;
		while (tz_heap_length(&h)) {
			int i = *(int *)tz_heap_peek(&h);
			eq &= prv >= i;
			prv = i;

			tz_heap_pop(&h);
		}

		tz_unit_test_should_pass(eq);
		tz_heap_destroy(&h);
	}

	{
		tz_heap h = tz_heap_create(sizeof(int), int_gt);

		tz_heap_put(&h, &(int){0});
		tz_heap_put(&h, &(int){1});
		tz_heap_put(&h, &(int){2});
		tz_heap_put(&h, &(int){3});

		bool eq = true;
		int prv = INT_MIN;
		while (tz_heap_length(&h)) {
			int i = *(int *)tz_heap_peek(&h);
			eq &= prv <= i;
			prv = i;

			tz_heap_pop(&h);
		}

		tz_unit_test_should_pass(eq);
		tz_heap_destroy(&h);
	}
}

int main(int argc, char const* argv[])
{
	tz_unit_test_start(argv[0]);
	test_smoke();
	return tz_unit_test_report();
}
