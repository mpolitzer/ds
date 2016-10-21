#ifndef TZ_STD_STRING_H
#define TZ_STD_STRING_H
#include <stdarg.h>
#include <ctype.h>
/* includes-here */

#ifdef TZ_STD_STRING_INLINE
#  define TZ_STD_STRING_M static inline
#  define TZ_STD_STRING_DECLARATIONS

#  ifndef TZ_STD_ARRAY_INLINE
#    define TZ_STD_ARRAY_INLINE TZ_STD_STRING_INLINE
#  endif

#else
#  define TZ_STD_STRING_M extern
#endif
#include "tz_array.h"
/* tz-includes-here */

#ifdef T
#error "Plese let me use T for a while..."
#endif
#define T tz_string

typedef tz_array tz_string; // type alias

/** Create an array by value */
TZ_STD_STRING_M T    tz_string_create(void);
TZ_STD_STRING_M void tz_string_format(T *me, const char *fmt, ...);
TZ_STD_STRING_M void tz_string_toupper(T *me);
TZ_STD_STRING_M void tz_string_tolower(T *me);
TZ_STD_STRING_M void tz_string_capitalize(T *me);

#ifdef TZ_STD_STRING_DECLARATIONS

TZ_STD_STRING_M T tz_string_create(void)
{
	return tz_array_create(sizeof(char));
}

TZ_STD_STRING_M T tz_string_dup(T *me)
{
	T s = tz_string_create();
	tz_array_pusn_data(&s, me->n, me->p);
	return s;
}

TZ_STD_STRING_M void tz_string_format(T *me, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);

	uint32_t n = vsnprintf((char *)me->p, me->max, fmt, ap)+1;

	if (n > me->max) {
		va_list ap_;
		va_start(ap_, fmt);
		tz_array_pushnz(me, n-me->max);
		me->n = vsnprintf((char *)me->p, me->max, fmt, ap_);
		va_end(ap_);
	} else {
		me->n = n;
	}

	va_end(ap);
}

TZ_STD_STRING_M void tz_string_toupper(T *me)
{
	for (uint32_t i=0; i<me->n; ++i)
		me->p[i] = toupper(me->p[i]);
}

TZ_STD_STRING_M void tz_string_toupper_s(T *me)
{
	for (uint32_t i=0; i<me->n; ++i)
		me->p[i] = toupper(me->p[i]);
}

TZ_STD_STRING_M T tz_string_tolower(T *me)
{
	for (uint32_t i=0; i<me->n; ++i)
		me->p[i] = toupper(me->p[i]);
}

TZ_STD_STRING_M void tz_string_capitalize(T *me)
{
	if (me->n)
		me->p[0] = toupper(me->p[0]);
}

#endif /* TZ_STD_STRING_DECLARATIONS */
#undef T
#endif /* TZ_STD_STRING_H */
