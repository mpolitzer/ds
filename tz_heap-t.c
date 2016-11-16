#define TZ_DS_HEAP_INLINE
#include "tz_heap.h"

bool int_lt(tz_heap *h, void *u, void *v)
{
	return *(int *)u < *(int *)v;
}

void test_smoke()
{
	tz_heap h = tz_heap_create(sizeof(int), int_lt);

	tz_heap_put(&h, &(int){0});
	tz_heap_put(&h, &(int){1});
	tz_heap_put(&h, &(int){2});
	tz_heap_put(&h, &(int){3});

	while (tz_heap_length(&h)) {
		printf("%d\n", *(int *)tz_heap_peek(&h));
		tz_heap_pop(&h);
	}

	tz_heap_destroy(&h);
}

int main(int argc, char const* argv[])
{
	test_smoke();
	return 0;
}
