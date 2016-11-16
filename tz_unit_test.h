#ifndef TZ_UNIT_TEST_H
#define TZ_UNIT_TEST_H
#include <stdbool.h>
#include <stdio.h>
/* includes-here */

#ifdef TZ_UNIT_TEST_INLINE
#  define TZ_UNIT_TEST_M static inline
#  define TZ_UNIT_TEST_DECLARATIONS

#  ifndef TZ_UTIL_INLINE
#    define TZ_UTIL_INLINE TZ_UNIT_TEST_INLINE
#  endif

#  ifndef TZ_ASSERT_INLINE
#    define TZ_ASSERT_INLINE TZ_UNIT_TEST_INLINE
#  endif

#else
#  define TZ_UNIT_TEST_M extern
#endif
/* tz-includes-here */

#define TZ_ANSI_COLOR_RED     "\x1b[31m"
#define TZ_ANSI_COLOR_GREEN   "\x1b[32m"
#define TZ_ANSI_COLOR_YELLOW  "\x1b[33m"
#define TZ_ANSI_COLOR_BLUE    "\x1b[34m"
#define TZ_ANSI_COLOR_MAGENTA "\x1b[35m"
#define TZ_ANSI_COLOR_CYAN    "\x1b[36m"
#define TZ_ANSI_COLOR_RESET   "\x1b[0m"

#define tz_unit_test_should_pass(exp)\
	tz_unit_test__should_pass(  exp,  #exp, __func__, __FILE__, __LINE__)
#define tz_unit_test_should_fail(exp)\
	tz_unit_test__should_pass(!(exp), #exp, __func__, __FILE__, __LINE__)

/* prefer one of the macros over this function */
TZ_UNIT_TEST_M void
tz_unit_test__should_pass(
                int passed,
                const char *exp,
                const char *func,
                const char *file,
                int line);

TZ_UNIT_TEST_M int tz_unit_test_report(void);

#ifdef  TZ_UNIT_TEST_DECLARATIONS // ---------------------------------------
static int tz_unit_test_passed, tz_unit_test_total;

#define TZ_UNIT_TEST_P TZ_ANSI_COLOR_GREEN "*" TZ_ANSI_COLOR_RESET
#define TZ_UNIT_TEST_F TZ_ANSI_COLOR_RED   "*" TZ_ANSI_COLOR_RESET

TZ_UNIT_TEST_M void
tz_unit_test__should_pass(
		int passed,
		const char *exp,
		const char *func,
		const char *file,
		int line)
{
	++tz_unit_test_total;
	if (passed) {
		++tz_unit_test_passed;
		fprintf(stdout, TZ_UNIT_TEST_P);
	} else {
		fprintf(stdout, TZ_UNIT_TEST_F);
		fprintf(stderr, "* %s:%d %s: `%s' failed.\n",
				file, line, func, exp);
		//tz_assert(0); // <- enable a gdb breakpoint here
	}
}

TZ_UNIT_TEST_M int
tz_unit_test_report(void)
{
	printf("\n");
	return tz_unit_test_total - tz_unit_test_passed;
}

TZ_UNIT_TEST_M void
tz_unit_test_start(const char *s)
{
	int n = printf("%s:", s);
	printf("%*.s[", 20-n, "");
}

#endif /* TZ_UNIT_TEST_DECLARATIONS */
#endif /* TZ_UNIT_TEST_H */

