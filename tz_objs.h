#ifndef TZ_DS_OBJS_H
#define TZ_DS_OBJS_H
// TODO: merge objs with obj

/* includes-here */

#ifdef TZ_DS_OBJS_INLINE
#  define TZ_DS_OBJS_M static inline
#  define TZ_DS_OBJS_DECLARATIONS

#  ifndef TZ_DS_ARRAY_INLINE
#    define TZ_DS_ARRAY_INLINE TZ_DS_OBJS_INLINE
#  endif

#  ifndef TZ_DS_QUEUE_INLINE
#    define TZ_DS_QUEUE_INLINE TZ_DS_OBJS_INLINE
#  endif

#  ifndef TZ_DS_OBJ_INLINE
#    define TZ_DS_OBJ_INLINE TZ_DS_OBJS_INLINE
#  endif

#else
#  define TZ_DS_OBJS_M extern
#endif
#include "tz_obj.h"
#include "tz_queue.h"
#include "tz_array.h"
/* tz-includes-here */

#ifdef T
#error "Plese let me use T for a while..."
#endif
#define T struct tz_objs_t
typedef T {
	tz_array seq;  // sequential part of the obj
	tz_queue free; // free list of objs to reuse
} tz_objs;

#define TZ_DS_OBJS_REUSE_THRESHOLD 1024
TZ_DS_OBJS_M T      tz_objs_create     ();
TZ_DS_OBJS_M void   tz_objs_destroy    (T *me);
TZ_DS_OBJS_M tz_obj tz_objs_new_obj    (T *me);
TZ_DS_OBJS_M void   tz_objs_del_obj    (T *me, tz_obj);
TZ_DS_OBJS_M bool   tz_objs_obj_isalive(T *me, tz_obj);

#ifdef TZ_DS_OBJS_DECLARATIONS

TZ_DS_OBJS_M T tz_objs_create()
{
	return (T) {
		.seq = tz_array_create(sizeof(uint8_t)),
		.free= tz_queue_create(sizeof(uint16_t)),
	};
}

TZ_DS_OBJS_M void tz_objs_destroy(T *me)
{
	tz_array_destroy(&me->seq);
	tz_queue_destroy(&me->free);
}

TZ_DS_OBJS_M tz_obj tz_objs_new_obj(T *me)
{
	uint8_t new_seq=0;
	uint32_t i=0;
	uint32_t n=tz_queue_length(&me->free);

	if (n > TZ_DS_OBJS_REUSE_THRESHOLD) { // will reuse from free list
		tz_queue_get(&me->free, &i);
		new_seq = *(uint8_t *)tz_array_sub_i(&me->seq, i);
	} else { // will create a new one
		tz_array_pushn_data(&me->seq, 1, &new_seq);
		i = tz_array_length(&me->seq)-1;
	}

	return tz_obj_create(i, new_seq);
}

TZ_DS_OBJS_M void tz_objs_del_obj(T *me, tz_obj obj)
{
	uint32_t i = obj.id;
	*(uint8_t *)tz_array_sub_i(&me->seq, i) = obj.seq+1;
	tz_queue_put(&me->free, &i);
}

TZ_DS_OBJS_M bool tz_objs_obj_isalive(T *me, tz_obj a)
{
	return *(uint8_t *)tz_array_sub_i(&me->seq, a.id) == a.seq;
}

#endif /* TZ_DS_OBJS_DECLARATIONS */
#undef T
#endif /* TZ_DS_OBJS_H */

