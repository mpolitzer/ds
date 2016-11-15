#ifndef TZ_DS_ATOMS_H
#define TZ_DS_ATOMS_H
/* includes-here */

#ifdef TZ_DS_ATOMS_INLINE
#  define TZ_DS_ATOMS_M static inline
#  define TZ_DS_ATOMS_DECLARATIONS

#  ifndef TZ_DS_MAP_INLINE
#    define TZ_DS_ARRAY_INLINE TZ_DS_ATOMS_INLINE
#  endif
#  ifndef TZ_DS_MAP_INLINE
#    define TZ_DS_MAP_INLINE TZ_DS_ATOMS_INLINE
#  endif

#else
#  define TZ_DS_ATOMS_M extern
#endif
#include "tz_array.h"
#include "tz_map.h"
/* tz-includes-here */

typedef struct {
	uint64_t id;
} tz_atom;

#ifdef T
#error "Plese let me use T for a while..."
#endif
#define T struct tz_atoms_t
typedef T {
	tz_map   to_index;
	tz_array as_list;
} tz_atoms;

TZ_DS_ATOMS_M T       tz_atoms_create ();
TZ_DS_ATOMS_M void    tz_atoms_init   (T *);
TZ_DS_ATOMS_M void    tz_atoms_destroy(T *);
//TZ_DS_ATOMS_M tz_atom tz_atoms_put    (T *, size_t n, char *p);
//TZ_DS_ATOMS_M tz_atom tz_atoms_get    (T *, size_t n, char *p);

#ifdef TZ_DS_ATOMS_DECLARATIONS

static inline uint32_t tz_atom__FNV1a(uint8_t *p, size_t n);

static inline uint32_t tz_atom__hash(tz_map *m, uint64_t u);
static inline bool tz_atom__eq(tz_map *m, uint64_t u, uint64_t v);

TZ_DS_ATOMS_M bool tz_atom_eq(tz_atom a, tz_atom b)
{
	return a.id == b.id;
}

TZ_DS_ATOMS_M T tz_atoms_create()
{
	return (T){
		.to_index = tz_map_create(tz_atom__hash, tz_atom__eq),
		.as_list  = tz_array_create(sizeof(char))
	};
}

TZ_DS_ATOMS_M void tz_atoms_init(T *me)
{
	tz_map_init  (&me->to_index, tz_atom__hash, tz_atom__eq);
	tz_array_init(&me->as_list, sizeof(char));
}

TZ_DS_ATOMS_M void tz_atoms_destroy(T *me)
{
	tz_map_destroy  (&me->to_index);
	tz_array_destroy(&me->as_list);
}

TZ_DS_ATOMS_M tz_atom tz_atoms_put(T *me, char *_s)
{
	uint64_t  s = (uint64_t)_s;
	uint64_t *i = tz_map_get(&me->to_index, s);
	if (i) return (tz_atom){ .id = *i};

	void    *dat = tz_array_pushn_data(&me->as_list, strlen(_s)+1, _s);
	uint64_t off = (uint8_t *)tz_array_base(&me->as_list) - (uint8_t *)dat;
	tz_map_put(&me->to_index, s, off);
	return (tz_atom){ .id = off };
}

TZ_DS_ATOMS_M const char *tz_atoms_get(T *me, tz_atom a)
{
	return tz_array_sub_i(&me->as_list, a.id);
}

/* -------------------------------------------------------------------------- */
static inline uint32_t tz_atom__FNV1a(uint8_t *p, size_t n)
{
	uint32_t hash = 2166136261lU;
	for (int i=0; i<n; ++i) {
		hash ^= p[i];
		hash *= 16777619lU;
	}
	return hash;
}

static inline uint32_t tz_atom__hash(tz_map *m, uint64_t u)
{
	uint8_t *s = (uint8_t *)u;
	size_t   n = strlen((char *)s);

	return tz_atom__FNV1a(s, n);
}

static inline bool tz_atom__eq(tz_map *m, uint64_t u, uint64_t v)
{
	char *s = (char *)u,
	     *t = (char *)v;

	return strcmp(s, t) == 0;
}

#endif /* TZ_DS_ATOMS_DECLARATIONS */
#undef T
#endif /* TZ_DS_ATOMS_H */
