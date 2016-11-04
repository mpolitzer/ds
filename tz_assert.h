#ifndef TZ_DS_ASSERT_H
#define TZ_DS_ASSERT_H
#include <stdio.h>
#include <signal.h>

#ifdef TZ_DS_ASSERT_INLINE
#  define TZ_DS_ASSERT_M static inline
#  define TZ_DS_ASSERT_DECLARATIONS
#else
#  define TZ_DS_ASSERT_M extern
#endif
/* tz-includes-here */

#define tz_assert(exp) tz_assert_(exp, #exp, __func__, __FILE__, __LINE__)
TZ_DS_ASSERT_M void tz_assert_(
		int passed,
		const char *exp,
		const char *func,
		const char *file,
		int line);

#ifdef TZ_DS_ASSERT_DECLARATIONS

TZ_DS_ASSERT_M void tz_assert_(
		int passed,
		const char *exp,
		const char *func,
		const char *file,
		int line)
{
	if (!passed) {
		fprintf(stderr, "* %s:%d %s: `%s' failed.\n",
		        file, line, func, exp);
		raise(SIGINT); /* <-- trigger a breakpoint in GDB */
	}
}

#endif /* TZ_DS_ASSERT_DECLARATIONS */
#undef T
#endif /* TZ_DS_ASSERT_H */

