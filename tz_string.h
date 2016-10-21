#ifndef TZ_STD_STRING_H
#define TZ_STD_STRING_H
#include <stdarg.h>
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


#ifdef TZ_STD_STRING_DECLARATIONS

TZ_STD_STRING_M T tz_string_create(void) {
	return tz_array_create(sizeof(char));
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

#endif /* TZ_STD_STRING_DECLARATIONS */
#undef T
#endif /* TZ_STD_STRING_H */
