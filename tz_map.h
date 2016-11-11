#ifndef TZ_DS_MAP_H
#define TZ_DS_MAP_H
/* Implementation of the hopscotch hashmap
 * The values are stored outside the structure.
 *
 * tz_map_put - returns the index+1 of the key passed in or 0 if a resize is
 *              required.
 * tz_map_get - returns the index+1 of the key passed in or 0 if the key is not
 *              in the hash.
 * tz_map_rm  - returns true if the key was in the hash.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
/* includes-here */

#ifdef TZ_DS_MAP_INLINE
#  define TZ_DS_MAP_M static inline
#  define TZ_DS_MAP_DECLARATIONS

#  ifndef TZ_DS_UTIL_INLINE
#    define TZ_DS_UTIL_INLINE TZ_DS_MAP_INLINE
#  endif

#else
#  define TZ_DS_MAP_M extern
#endif
/* tz-includes-here */

#ifdef T
#error "Plese let me use T for a while..."
#endif

#define T struct tz_map_t
typedef T tz_map;

typedef uint32_t (*tz_map_hash)(T *me, uint64_t u);
typedef bool     (*tz_map_eq)  (T *me, uint64_t u, uint64_t v);
T {
	tz_map_hash hash;
	tz_map_eq  eq;
	void *mem, *user;
	uint32_t n, max;
};

TZ_DS_MAP_M void  tz_map_init   (T *me, tz_map_hash hash, tz_map_eq eq);
TZ_DS_MAP_M T     tz_map_create (tz_map_hash hash, tz_map_eq eq);
TZ_DS_MAP_M void  tz_map_destroy(T *me);

TZ_DS_MAP_M uint64_t *tz_map_put(T *me, uint64_t key, uint64_t val);
TZ_DS_MAP_M uint64_t *tz_map_get(T *me, uint64_t key);
TZ_DS_MAP_M bool      tz_map_rm (T *me, uint64_t key);

TZ_DS_MAP_M void     tz_map_rehash(T *me, uint32_t max);
TZ_DS_MAP_M void     tz_map_print_all(T *me);

#ifdef TZ_DS_MAP_DECLARATIONS
static inline uint64_t *tz_map__keys(T *me);
static inline uint64_t *tz_map__vals(T *me);
static inline uint64_t *tz_map__hops(T *me);
static inline uint64_t *tz_map__used(T *me);

static inline uint32_t tz_map__mod(uint32_t i, uint32_t n);
static inline uint32_t tz_map__find_slot(T *me, uint64_t lookup, uint32_t s);
static inline uint32_t tz_map__distance_to_first_empty_slot(T *me, uint32_t s);
static inline void     tz_map__resize(T *me, uint64_t n);

TZ_DS_MAP_M T tz_map_create(tz_map_hash hash, tz_map_eq eq)
{
	return (T){
		.hash= hash,
		.eq = eq,
	};
}

TZ_DS_MAP_M void tz_map_init(T *me, tz_map_hash hash, tz_map_eq eq)
{
	me->hash = hash;
	me->eq  = eq;
	me->n    = 0;
	me->max  = 0;
}

TZ_DS_MAP_M void tz_map_destroy(T *me)
{
	//free(me->bs);
}

TZ_DS_MAP_M uint64_t *tz_map_put(T *me, uint64_t key, uint64_t val)
{
	if (me->max == 0) {
		tz_map__resize(me, 7);
	}
	uint32_t hash  = me->hash(me, key),
		 start = hash % me->max,
		 entry = tz_map__find_slot(me, key, start);
	if (entry--) return &tz_map__vals(me)[entry];

	uint32_t max = me->max,
		 p = start,
		 d = tz_map__distance_to_first_empty_slot(me, p),
		 pd;
	if (d-- == 0) {
		tz_map_rehash(me, 2*me->max+1);
		return tz_map_put(me, key, val);
	}
	while (d > 63) {
		pd = tz_map__mod(p+d, max);
		uint32_t base, i, j;
		for (i=63, j=0; i>0; --i) {
			base = tz_map__mod(pd-i+max, max);
			uint64_t hop_info = tz_map__hops(me)[base];
			if (hop_info && i > (j=__builtin_ctzll(hop_info))) {
				entry = tz_map__mod(pd-i+j+max, max);
				break;
			}
		}
		// base  = mod(pd-i  +max, max)
		// entry = mod(pd-i+j+max, max)
		if (i == 0) {
			tz_map_rehash(me, 2*me->max+1);
			return tz_map_put(me, key, val);
		}
		tz_map__keys(me)[pd]    = tz_map__keys(me)[entry];
		tz_map__vals(me)[pd]    = tz_map__vals(me)[entry];

		tz_map__used(me)[entry/64] &=~(1ull<<(entry&0x3f));
		tz_map__used(me)[pd   /64] |= (1ull<<(pd   &0x3f));

		tz_map__hops(me)[base]&=~(1ull<<(j));
		tz_map__hops(me)[base]|= (1ull<<(i));

		d = tz_map__mod(entry-p+max, max);
	}
	pd = tz_map__mod(p+d, max);
	tz_map__keys(me)[pd]     = key;
	tz_map__vals(me)[pd]     = val;
	tz_map__used(me)[pd/64] |= (1ull<<(pd & 0x3f));
	tz_map__hops(me)[p]     |= (1ull<<d);
	return &tz_map__vals(me)[pd];
}

TZ_DS_MAP_M uint64_t *tz_map_get(T *me, uint64_t key)
{
	uint32_t hash  = me->hash(me, key),
		 start = hash % me->max;
	uint32_t slot  = tz_map__find_slot(me, key, start);
	if (slot-- == 0) return NULL;
	return &tz_map__vals(me)[slot];
}

TZ_DS_MAP_M bool tz_map_rm(T *me, uint64_t key)
{
	uint32_t hash = me->hash(me, key),
		 start= hash % me->max,
		 entry= tz_map__find_slot(me, key, start);
	if (entry-- == 0) return false;

	tz_map__hops(me)[start]    &=~(1ull<<(entry-start));
	tz_map__used(me)[entry/64] &=~(1ull<<entry&0x3f);
	return true;
}

TZ_DS_MAP_M void tz_map_rehash(T *me, uint32_t max)
{
	T old = *me;
	tz_map__resize(me, max);

	//printf("<rehash>\n");
	for (uint32_t i=0; i<old.max; ++i) {
		uint64_t key = tz_map__keys(&old)[i];
		uint64_t val = tz_map__vals(&old)[i];
		if (tz_map__keys(&old)[i]) {
			tz_map_put(me, key, val);
		}
	}

	free(old.mem);
}

TZ_DS_MAP_M void tz_map_print_all(T *me)
{
	//struct tz_map_bucket_t *b, *bs = me->bs;
	uint32_t max = me->max;

	for (int i=0; i<max; ++i) {
		uint64_t key = tz_map__keys(me)[i];
		if (key) {
			printf("%04d: %-10s: ", i, (char *)key);
		}
		else {
			printf("%04d: %-10s: ", i, "");
		}
		//uint64_t hop = tz_map__hops(me)[i];
		//tz_util_printb(hop);
	}
}


/* -------------------------------------------------------------------------- */
static inline uint64_t *tz_map__keys(T *me)
{
	return (uint64_t *)((uint8_t *)me->mem + (0*sizeof(uint64_t))*me->max);
}

static inline uint64_t *tz_map__vals(T *me)
{
	return (uint64_t *)((uint8_t *)me->mem + (1*sizeof(uint64_t))*me->max);
}

static inline uint64_t *tz_map__hops(T *me)
{
	return (uint64_t *)((uint8_t *)me->mem + (2*sizeof(uint64_t))*me->max);
}

static inline uint64_t *tz_map__used(T *me)
{
	return (uint64_t *)((uint8_t *)me->mem + (3*sizeof(uint64_t))*me->max);
}

static inline void tz_map__resize(T *me, uint64_t n)
{
	size_t bytes =
		(n        * sizeof(uint64_t)) + // keys (64bits p/ entry)
		(n        * sizeof(uint64_t)) + // vals (64bits p/ entry)
		(n        * sizeof(uint64_t)) + // hops (64bits p/ entry)
		((n/64+1) * sizeof(uint64_t));  // used (1bit   p/ entry)
	me->max = n;
	me->mem = memset(malloc(bytes), 0, bytes);
}

static inline uint32_t tz_map__find_slot(T *me, uint64_t lookup, uint32_t s)
{
	uint64_t hop_info = tz_map__hops(me)[s];
	while (hop_info) {
		uint32_t nxt   = __builtin_ctzll(hop_info),
			 entry = tz_map__mod(s+nxt, me->max);
		if (me->eq(me, tz_map__keys(me)[entry], lookup))
			return entry+1;
		hop_info &=~(1ull<<nxt);
	}
	return 0;
}

static inline uint32_t tz_map__distance_to_first_empty_slot(T *me, uint32_t s)
{
	uint32_t n = me->max;

	for (uint32_t i=0; i<n; ++i) {
		uint64_t bit = tz_map__mod(s+i, n);
		if (!(tz_map__used(me)[bit/64] & (1ull<<(bit&0x3f))))
			return i+1;
	}
	return 0;
}

static inline uint32_t tz_map__mod(uint32_t i, uint32_t n)
{
	return i < n? i: i-n;
}

#endif /* TZ_DS_MAP_DECLARATIONS */
#undef T
#endif /* TZ_DS_MAP_H */

