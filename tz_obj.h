#ifndef TZ_DS_OBJ_H
#define TZ_DS_OBJ_H
#include <stdint.h>
#include <stdio.h>
/* includes-here */

#ifdef TZ_DS_OBJ_INLINE
#  define TZ_DS_OBJ_M static inline
#  define TZ_DS_OBJ_DECLARATIONS
#else
#  define TZ_DS_OBJ_M extern
#endif
/* tz-includes-here */

#ifdef T
#error "Plese let me use T for a while..."
#endif
#define T struct tz_obj_t

typedef T {
	uint16_t id;
	uint8_t seq;
} tz_obj;

TZ_DS_OBJ_M T        tz_obj_create(uint16_t i, uint8_t seq);
TZ_DS_OBJ_M uint32_t tz_obj_as_u32(T me);
TZ_DS_OBJ_M uint16_t tz_obj_index (T me);
TZ_DS_OBJ_M uint8_t  tz_obj_seq   (T me);
TZ_DS_OBJ_M void     tz_obj_print (T me);
TZ_DS_OBJ_M uint32_t tz_obj_hash(uint64_t p);
TZ_DS_OBJ_M bool     tz_obj_cmp (uint64_t u, uint64_t v);

#ifdef TZ_DS_OBJ_DECLARATIONS

TZ_DS_OBJ_M T tz_obj_create(uint16_t i, uint8_t seq)
{
	return (T) { .id = i, .seq = seq };
}

TZ_DS_OBJ_M uint32_t tz_obj_as_u32(T me)
{
	return (me.id << 8) + me.seq;
}

TZ_DS_OBJ_M uint16_t tz_obj_index(T me)
{
	return me.id;
}

TZ_DS_OBJ_M uint8_t tz_obj_seq(T me)
{
	return me.seq;
}

TZ_DS_OBJ_M void tz_obj_print(T me)
{
	printf("id: %d, seq: %d\n", me.id, me.seq);
}

TZ_DS_OBJ_M uint32_t tz_obj_hash(uint64_t p)
{
	return p;
}

TZ_DS_OBJ_M bool tz_obj_cmp(uint64_t u, uint64_t v)
{
	return (uint64_t)u == (uint64_t)v;
}

#endif /* TZ_DS_OBJ_DECLARATIONS */
#undef T
#endif /* TZ_DS_OBJ_H */

