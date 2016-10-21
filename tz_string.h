#ifndef TZ_STD_STRING_H
#define TZ_STD_STRING_H
/* includes-here */

#ifdef TZ_STD_STRING_INLINE
#  define TZ_STD_STRING_M static inline
#  define TZ_STD_STRING_DECLARATIONS
#else
#  define TZ_STD_STRING_M extern
#endif
#include "tz_array.h"
/* tz-includes-here */

#ifdef T
#error "Plese let me use T for a while..."
#endif
#define T struct tz_string_t

//typedef T tz_array;
//
//typedef T {
//
//} tz_string;

#ifdef TZ_STD_STRING_DECLARATIONS

#endif /* TZ_STD_STRING_DECLARATIONS */
#undef T
#endif /* TZ_STD_STRING_H */
