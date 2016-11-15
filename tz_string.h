#ifndef TZ_DS_STRING_H
#define TZ_DS_STRING_H
#include <stdarg.h>
#include <ctype.h>
/* includes-here */

#ifdef TZ_DS_STRING_INLINE
#  define TZ_DS_STRING_M static inline
#  define TZ_DS_STRING_DECLARATIONS

#  ifndef TZ_DS_ARRAY_INLINE
#    define TZ_DS_ARRAY_INLINE TZ_DS_STRING_INLINE
#  endif

#else
#  define TZ_DS_STRING_M extern
#endif
#include "tz_array.h"
/* tz-includes-here */

#ifdef T
#error "Plese let me use T for a while..."
#endif
#define T tz_string

typedef tz_array tz_string; // type alias

#define TZ_STRING_TO_FMT "%.*s"
#define tz_string_to_fmt(s) (s)->n, (s)->p

/** Create an array by value */
TZ_DS_STRING_M T        tz_string_create   (void);
TZ_DS_STRING_M T        tz_string_create_b (const char *s, size_t n);
TZ_DS_STRING_M T        tz_string_create_s (const char *s);
TZ_DS_STRING_M void     tz_string_destroy  (T *me);
TZ_DS_STRING_M T        tz_string_dup      (T *me);
TZ_DS_STRING_M T       *tz_string_format   (T *me, const char *fmt, ...);
TZ_DS_STRING_M T       *tz_string_toupper  (T *me);
TZ_DS_STRING_M T       *tz_string_toupper_s(T *me, const char *s);
TZ_DS_STRING_M tz_array tz_string_split    (char *s);

#ifdef TZ_DS_STRING_DECLARATIONS

TZ_DS_STRING_M T tz_string_create(void)
{
	return tz_array_create(sizeof(char));
}

TZ_DS_STRING_M T tz_string_create_b (const char *s, size_t n)
{
	T me = tz_string_create();
	tz_array_pushn_data(&me, n, (void *)s);
	return me;
}

TZ_DS_STRING_M T tz_string_create_s(const char *s)
{
	T me = tz_string_create();
	tz_array_pushn_data(&me, strlen(s), (void *)s);
	return me;
}

TZ_DS_STRING_M void tz_string_destroy(T *me)
{
	tz_array_destroy(me);
}

TZ_DS_STRING_M T tz_string_dup(T *me)
{
	T new = tz_string_create();
	tz_array_pushn_data(&new, me->n, me->p);
	return new;
}

TZ_DS_STRING_M T *tz_string_format(T *me, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);

	uint32_t n = vsnprintf((char *)me->p, me->max, fmt, ap)+1;

	if (n > me->max) {
		va_list ap_;
		va_start(ap_, fmt);
		tz_array_pushnz(me, n-me->max);
		me->n = vsnprintf((char *)me->p, me->max, fmt, ap_)+1;
		va_end(ap_);
	} else {
		me->n = n;
	}

	va_end(ap);
	return me;
}

TZ_DS_STRING_M T *tz_string_toupper(T *me)
{
	for (uint32_t i=0; i<me->n; ++i)
		me->p[i] = toupper(me->p[i]);
	return me;
}

TZ_DS_STRING_M T *tz_string_tolower(T *me)
{
	for (uint32_t i=0; i<me->n; ++i)
		me->p[i] = tolower(me->p[i]);
	return me;
}

TZ_DS_STRING_M T *tz_string_toupper_s(T *me, const char *s)
{
	size_t n = strlen(s);
	tz_array_reserve(me, n);
	me->n = n;

	for (uint32_t i=0; i<me->n; ++i)
		me->p[i] = toupper(s[i]);

	return me;
}

/**
 *
 * return: array<string> */
TZ_DS_STRING_M tz_array tz_string_split(char *line)
{
	tz_array ss = tz_array_create(sizeof(tz_string));
	char *s=line;

	while (isspace(*s))
		s++;

	for (; *s; ++s) {
		char *c=s;

		if (*c == '"') {
			s = ++c;
			while (*s && *++s != '"')
				;
			if (*s == '\0')
				return ss; // unterminated string
			tz_string *x = tz_array_pushn(&ss, 1);
			*x = tz_string_create_b(c, s-c);
			continue;
		}

		if (!isspace(*s)) {
			while (*s && !isspace(*++s))
				;

			//printf("[%.*s][%ld]\n", (int)(s-c), c, s-c);
			tz_string *x = tz_array_pushn(&ss, 1);
			*x = tz_string_create_b(c, s-c);
			continue;
		}
	}
	return ss;
}

#endif /* TZ_DS_STRING_DECLARATIONS */
#undef T
#endif /* TZ_DS_STRING_H */
