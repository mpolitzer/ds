#ifndef TZ_DS_HEAP_H
#define TZ_DS_HEAP_H
/* includes-here */

#ifdef TZ_DS_HEAP_INLINE
#  define TZ_DS_HEAP_M static inline
#  define TZ_DS_HEAP_DECLARATIONS

#  ifndef TZ_DS_ARRAY_INLINE
#    define TZ_DS_ARRAY_INLINE TZ_DS_HEAP_INLINE
#  endif

#else
#  define TZ_DS_HEAP_M extern
#endif
#include "tz_array.h"
/* tz-includes-here */

#ifdef T
#error "Plese let me use T for a while..."
#endif
#define T struct tz_heap_t
typedef T tz_heap;

typedef bool (*tz_heap_lt)(T *me, void *u, void *v);
T {
	tz_array   data;
	tz_heap_lt lt;   // '<'
};

TZ_DS_HEAP_M void     tz_heap_init   (T *me, size_t elm_size, tz_heap_lt lt);
TZ_DS_HEAP_M T        tz_heap_create (size_t elm_size, tz_heap_lt lt);
TZ_DS_HEAP_M void     tz_heap_destroy(T *me);
TZ_DS_HEAP_M uint32_t tz_heap_length (T *me);

TZ_DS_HEAP_M void     tz_heap_put    (T *me, void *dat);
TZ_DS_HEAP_M uint32_t tz_heap_get    (T *me, void *dat);

TZ_DS_HEAP_M void    *tz_heap_peek   (T *me);
TZ_DS_HEAP_M uint32_t tz_heap_pop    (T *me);

#ifdef TZ_DS_HEAP_DECLARATIONS
static inline void tz_heap__move_up  (T *me, uint32_t i, uint32_t n);
static inline void tz_heap__move_down(T *me, uint32_t i, uint32_t n);

TZ_DS_HEAP_M void tz_heap_init(T *me, size_t elm_size, tz_heap_lt lt)
{
	tz_array_init(&me->data, elm_size);
	me->lt = lt;
}

TZ_DS_HEAP_M T tz_heap_create(size_t elm_size, tz_heap_lt lt)
{
	return (T){
		.data = tz_array_create(elm_size),
		.lt   = lt,
	};
}

TZ_DS_HEAP_M void tz_heap_destroy(T *me)
{
	tz_array_destroy(&me->data);
}

TZ_DS_HEAP_M uint32_t tz_heap_length (T *me)
{
	tz_array *a = &me->data;
	return tz_array_length(a);
}

TZ_DS_HEAP_M void tz_heap_put(T *me, void *dat)
{
	tz_array *a = &me->data;
	tz_array_pushn_data(a, 1, dat);
	uint32_t n = tz_array_length(a);
	tz_heap__move_up(me, n-1, n);
}

TZ_DS_HEAP_M uint32_t tz_heap_get(T *me, void *dat)
{
	tz_array *a = &me->data;
	tz_array_load(a, 0, dat);
	return tz_array_length(a);
}


TZ_DS_HEAP_M void *tz_heap_peek(T *me)
{
	tz_array *a = &me->data;
	return tz_array_sub_i(a, 0);
}

TZ_DS_HEAP_M uint32_t tz_heap_pop(T *me)
{
	tz_array *a = &me->data;
	tz_array_drop(a, 0);
	tz_heap__move_down(me, 0, tz_array_length(a));
	return tz_array_length(a);
}

/* -------------------------------------------------------------------------- */
#define tz_heap__parent(i)     ((i)/2)
#define tz_heap__leftchild(i)  (2*((i)+1)-1)
#define tz_heap__rightchild(i) (2*((i)+1)-0)

static inline void tz_heap__move_up(T *me, uint32_t i, uint32_t n)
{
	tz_array *a = &me->data;
	uint32_t  p = tz_heap__parent(i);

	if (i < n && me->lt(me, tz_array_sub_i(a, p),
				tz_array_sub_i(a, i))) {
		tz_array_swap(a, i, p);
		tz_heap__move_up(me, p, n);
	}
}

static inline void tz_heap__move_down(T *me, uint32_t i, uint32_t n)
{
	tz_array *a = &me->data;

	uint32_t l = tz_heap__leftchild (i);
	if (l < n && me->lt(me, tz_array_sub_i(a, i),
				tz_array_sub_i(a, l))) {
		tz_array_swap(a, i, l);
		tz_heap__move_down(me, l, n);
	}

	uint32_t r = tz_heap__rightchild(i);
	if (r < n && me->lt(me, tz_array_sub_i(a, i),
				tz_array_sub_i(a, r))) {
		tz_array_swap(a, i, r);
		tz_heap__move_down(me, r, n);
	}
}

#endif /* TZ_DS_HEAP_DECLARATIONS */
#undef T
#endif /* TZ_DS_HEAP_H */
