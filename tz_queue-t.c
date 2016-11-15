#define TZ_UNIT_TEST_INLINE
#include "tz_unit_test.h"

#define TZ_DS_QUEUE_INLINE
#include "tz_queue.h"

#define SIZE(x) sizeof(x)/sizeof(*x)

void test_smoke()
{
	int i;
	tz_queue q = tz_queue_create(sizeof(int));
	tz_queue_put_t(&q, int, 42);
	tz_queue_get  (&q, &i);

	tz_unit_test_should_pass(i == 42);
	tz_queue_destroy(&q);
}

void test_loop()
{
	bool eq  = true;
	int is[] = { 0, 1, 2, 3, 4, 5 };
	int os[] = { 0, 1, 2, 3, 4, 5 };
	tz_queue q = tz_queue_create(sizeof(int));

	tz_queue_put_t(&q, int, is[0]);
	tz_queue_put_t(&q, int, is[1]);
	tz_queue_put_t(&q, int, is[2]);
	tz_queue_put_t(&q, int, is[3]);
	tz_queue_put_t(&q, int, is[4]);
	tz_queue_put_t(&q, int, is[5]);

	for (int i=0; i<SIZE(os); ++i) {
		int v = *(int *)tz_queue_peek(&q);
		eq &= os[i] == v;
		tz_queue_pop(&q);
	}
	tz_unit_test_should_pass(eq);
	tz_queue_destroy(&q);
}

int main(int argc, char const* argv[])
{
	tz_unit_test_start(argv[0]);
	test_smoke();
	test_loop();
	return tz_unit_test_report();
}
