#include <stdio.h>
#include <stdlib.h>

#define TZ_JOBS_INLINE
#include "tz_jobs.h"

typedef struct X X;
struct X {
	tz_job _;
	int i;
};

void X_init(X *x, void (*f)(tz_job *j), int i)
{
	tz_job_init(&x->_, f);
	x->i = i;
}

void print(tz_job *j)
{
	X *x = (X *)j;
	printf("%d\n", x->i);
}

void check_put()
{
	tz_jobs g_js;
	tz_jobs_init(&g_js);

	X g_j[10];
	for (int i=0; i<10; ++i) {
		X_init(&g_j[i], print, i);
	}

	for (int i=0; i<1000; ++i) {
		int v = rand() % 11;
		switch (v) {
		default: tz_jobs_put(&g_js, (tz_job *)&g_j[v]);
			 printf("%d", v);
			 break;
		case 10: printf("\n");
			 tz_jobs_run(&g_js);
			 printf("\n");
			 break;
		}
	}

	printf("\n");
	tz_jobs_run(&g_js);
	printf("\n");
}

int main(int argc, char const* argv[])
{
	check_put();
	return 0;
}
