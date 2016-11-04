#ifndef TZ_JOBS_H
#define TZ_JOBS_H
#include <stddef.h>
/* includes-here */

#ifdef TZ_JOBS_INLINE
#  define TZ_JOBS_M static inline
#  define TZ_JOBS_DECLARATIONS
#else
#  define TZ_JOBS_M extern
#endif
/* tz-includes-here */

typedef struct tz_job_t {
	struct tz_job_t *nxt;
	void (*fn)(struct tz_job_t *me);
} tz_job;

#ifdef T
#error "Plese let me use T for a while..."
#endif
#define T struct tz_jobs_t
typedef T {
	tz_job *head, **tail;
} tz_jobs;

TZ_JOBS_M void tz_job_init (tz_job *me, void (*f)(struct tz_job_t *));
TZ_JOBS_M void tz_jobs_init(tz_jobs *me);
TZ_JOBS_M void tz_jobs_put (tz_jobs *me, tz_job *job);
TZ_JOBS_M void tz_jobs_run (tz_jobs *me);

#ifdef TZ_JOBS_DECLARATIONS

TZ_JOBS_M void tz_job_init(tz_job *me, void (*f)(struct tz_job_t *))
{
	me->fn  = f;
	me->nxt = me;
}

TZ_JOBS_M void tz_jobs_init(tz_jobs *me)
{
	me->head = NULL;
	me->tail = &me->head;
}

TZ_JOBS_M void tz_jobs_put(tz_jobs *me, tz_job *job)
{
	if (job->nxt != job) return;

	job->nxt  = NULL;
	*me->tail = job;
	 me->tail =&job->nxt;
}

TZ_JOBS_M void tz_jobs_run(tz_jobs *me)
{
	tz_job *lst = me->head;
	tz_jobs_init(me);

	while (lst) { // nodes added during this loop will run on the next call
		tz_job *j = lst;
		lst = lst->nxt;
		j->nxt = j;
		j->fn(j);
	}
}

#endif /* TZ_JOBS_DECLARATIONS */
#undef T
#endif /* TZ_JOBS_H */
