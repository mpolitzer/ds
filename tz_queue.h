#ifndef TZ_DS_QUEUE_H
#define TZ_DS_QUEUE_H
#include <stdint.h>
/* includes-here */

#ifdef TZ_DS_QUEUE_INLINE
#  define TZ_DS_QUEUE_M static inline
#  define TZ_DS_QUEUE_DECLARATIONS

#  ifndef TZ_DS_ARRAY_INLINE
#    define TZ_DS_ARRAY_INLINE TZ_DS_QUEUE_INLINE
#  endif

#else
#  define TZ_DS_QUEUE_M extern
#endif
#include "tz_array.h"
/* tz-includes-here */

#ifdef T
#error "Plese let me use T for a while..."
#endif
#define T struct tz_queue_t
typedef T {
	uint32_t head;
	tz_array data;
} tz_queue;

TZ_DS_QUEUE_M void     tz_queue_init   (T *me, size_t elm_size);
TZ_DS_QUEUE_M T        tz_queue_create (size_t elm_size);
TZ_DS_QUEUE_M void     tz_queue_destroy(T *me);
TZ_DS_QUEUE_M void     tz_queue_put    (T *me, void *in);
TZ_DS_QUEUE_M uint32_t tz_queue_get    (T *me, void *out);

TZ_DS_QUEUE_M void    *tz_queue_peek   (T *me);
TZ_DS_QUEUE_M uint32_t tz_queue_pop    (T *me);

#ifdef TZ_DS_QUEUE_DECLARATIONS

TZ_DS_QUEUE_M void tz_queue_init(T *me, size_t elm_size)
{
	me->head = 0;
	tz_array_init(&me->data, elm_size);
}

TZ_DS_QUEUE_M T tz_queue_create(size_t elm_size)
{
	return (T){
		.head = 0,
		.data = tz_array_create(elm_size)
	};
}

TZ_DS_QUEUE_M void tz_queue_destroy(T *me)
{
	tz_array_destroy(&me->data);
}

TZ_DS_QUEUE_M void *tz_queue_peek(T *me)
{
	return tz_array_sub_i(&me->data, me->head);
}

TZ_DS_QUEUE_M void tz_queue_put(T *me, void *in)
{
	tz_array_pushn_data(&me->data, 1, in);
}
#define tz_queue_put_t(m, t, d) \
	tz_queue_put(m, &(t){d})

TZ_DS_QUEUE_M uint32_t tz_queue_pop(T *me)
{
	uint32_t n  = tz_array_length(&me->data);
	if (me->head > n * 3/4) {
		tz_array_dropn_ord(&me->data, me->head, 0);
		me->head = 0;
	}
	me->head++;
	return tz_array_length(&me->data) - me->head;
}

TZ_DS_QUEUE_M uint32_t tz_queue_get(T *me, void *out)
{
	uint32_t n  = tz_array_length(&me->data);
	if (me->head > n * 3/4) {
		tz_array_dropn_ord(&me->data, me->head, 0);
		me->head = 0;
	}
	tz_array_load(&me->data, me->head++, out);
	return tz_array_length(&me->data) - me->head;
}

TZ_DS_QUEUE_M uint32_t tz_queue_length(T *me)
{
	uint32_t n  = tz_array_length(&me->data);
	return n-me->head;
}

#endif /* TZ_DS_QUEUE_DECLARATIONS */
#undef T
#endif /* TZ_DS_QUEUE_H */
