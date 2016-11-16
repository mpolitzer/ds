#include <stdio.h>
#include <stdlib.h>

#define TZ_UNIT_TEST_INLINE
#include "tz_unit_test.h"

#define TZ_JOBS_INLINE
#include "tz_jobs.h"

#define SIZE(x) sizeof(x)/sizeof(*x)

typedef struct {
	tz_job _;

	int   i;
	void *dat;
	bool  eq;
} my_job_t;

void my_job_fn(tz_job *_j)
{
	my_job_t *j = (my_job_t *)_j;
	j->eq &= j - (my_job_t *)j->dat == j->i;
}

void my_job_init(my_job_t *me, void *dat, int i)
{
	tz_job_init(&me->_, my_job_fn);
	me->dat = dat;
	me->i   = i;
	me->eq  = true;
}

void test_smoke()
{
	tz_jobs js;
	tz_jobs_init(&js);

	my_job_t j = { .dat = &j, .i = 0, .eq = true };

	tz_job_init(&j._, my_job_fn);
	tz_jobs_put(&js, &j._);

	tz_unit_test_should_pass(j.eq);
	tz_jobs_run(&js);
}

void test_join()
{
	tz_jobs js[2];
	tz_jobs_init(&js[0]);
	tz_jobs_init(&js[1]);

	my_job_t j[10];
	for (int i=0; i<SIZE(j)/2; ++i) {
		my_job_init(&j[i], j, i);
		tz_jobs_put(&js[0], &j[i]._);
	}

	for (int i=SIZE(j)/2; i<SIZE(j); ++i) {
		my_job_init(&j[i], j, i);
		tz_jobs_put(&js[1], &j[i]._);
	}

	tz_jobs_join(&js[0], &js[1]);
	tz_jobs_run(&js[0]);
	//tz_jobs_run(&js[1]);

	bool eq=true;
	for (int i=0; i<SIZE(j)/2; ++i) {
		eq &= j[i].eq;
	}

	tz_unit_test_should_pass(eq);
}

int main(int argc, char const* argv[])
{
	tz_unit_test_start(argv[0]);
	test_smoke();
	test_join();
	return tz_unit_test_report();
}
