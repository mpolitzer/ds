#ifndef TZ_DS_ARRAY_H
#define TZ_DS_ARRAY_H
/** @file
 * @defgroup tz_array_t
 * @{
 *
 * @brief rezisable array (similar to C++ STD vector)
 *
 * complexity:
 * - pushn        -   O(1) amortizing on alocation cost.
 * - popn         -   O(1) amortizing on alocation cost.
 * - sub_i        -   O(1).
 * - getn/setn    -   O(n).
 * - swapn        - 2*O(n).
 * - moven        -   O(n).
 * - insn         -   O(n) from point of insertion to end of array.
 * - dropn        -   O(n).
 * - dropn_ord    -   O(n) from point of removal to end of array.
 * - clear        -   O(1). */

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* includes-here */

#ifdef TZ_DS_ARRAY_INLINE
#  define TZ_DS_ARRAY_M static inline
#  define TZ_DS_ARRAY_DECLARATIONS

#  ifndef TZ_DS_UTIL_INLINE
#    define TZ_DS_UTIL_INLINE TZ_DS_ARRAY_INLINE
#  endif

#  ifndef TZ_DS_ASSERT_INLINE
#    define TZ_DS_ASSERT_INLINE TZ_DS_ARRAY_INLINE
#  endif

#else
#  define TZ_DS_ARRAY_M extern
#endif
#include "tz_assert.h"
/* tz-includes-here */

#ifdef T
#error "Plese let me use T for a while..."
#endif
#define T struct tz_array_t

/* TODO: fix this padding. */
typedef T {
	uint32_t n,      ///< amount of elements in the array
	         max;    ///< available elements wihtout reallocation
	size_t elm_size; ///< size in bytes for each element

	uint8_t *p; ///< pointer holding the memory
} tz_array;

/** Create an array by value
 *
 * @param elm_size - size in bytes for each element in the array
 * @note           - it is garanteed that no alocation happens here! */
TZ_DS_ARRAY_M T tz_array_create(size_t elm_size);

/** Initialize an alocated array (equivalent to create)
 *
 * @param me       - alocated array
 * @param elm_size - size in bytes for each element in the array
 * @note           - it is garanteed that no alocation happens here! */
TZ_DS_ARRAY_M void tz_array_init(T *me, size_t elm_size);

/** Create an array by value from initial values.
 *
 * @param elm_size - size in bytes for each element in the array
 * @param p        - pointer on the heap. if not on the heap a usage pattern
 *                   that garantees to never call a function that may realloc.
 *                   (Avoid doing this like the plague)
 * @param n        - number of elements in p.
 * @param elm_size - size of a single element in p.
 *
 * @note           - pointers 'p' on the stack are very tricky to use correcly.
 * @note           - it is garanteed that no alocation happens here! */
TZ_DS_ARRAY_M T tz_array_build(void *p, uint32_t n, size_t elm_size);

/* Destroy the array
 *
 * @param me - array instance
 * @note     - frees the memory, will leak the contents if not dealt with. */
TZ_DS_ARRAY_M void tz_array_destroy(T *me);

/* Reserve at least n elements to work with without reallocation on push
 *
 * @var me - array instance
 * @var n  - make the array fit at least n elements without reallocation.
 * @note   - This will invalidate pointers */
TZ_DS_ARRAY_M bool tz_array_reserve(T *me, uint32_t n);

/* Grab a pointer to the base of the array
 *
 * @me  - array instance
 * @ret - pointer to the first element */
TZ_DS_ARRAY_M void *tz_array_base(T *me);

/* Number of elements in the array
 *
 * @me  - array instance
 * @ret - number of elements */
TZ_DS_ARRAY_M uint32_t tz_array_length(T *me);

/* Push n elements of space into the end.
 *
 * @me  - array instance
 * @n   - number of elements to make available (at the end).
 * @ret - a pointer to the base of this memory.
 *
 * @note - This will invalidate pointers */
TZ_DS_ARRAY_M void *tz_array_pushn(T *me, uint32_t n);
#define tz_array_push(me) tz_array_pushn(me, 1)

/* same as pushn but zeroing the new values */
TZ_DS_ARRAY_M void *tz_array_pushnz(T *me, uint32_t n);

TZ_DS_ARRAY_M void *tz_array_pushn_data(T *me, uint32_t n, void *in);

/* Pop n elements from the end
 *
 * @me  - array instance.
 * @n   - amount of elements to remove (from the end).
 * @ret - amount of elements actually removed. (0 for an empty array)
 *
 * @note - This will invalidate pointers */
TZ_DS_ARRAY_M uint32_t tz_array_popn(T *me, uint32_t n);
#define tz_array_pop(me) tz_array_popn(me, 1)

/* Pop n elements from the end and store them on out.
 *
 * @me  - array instance.
 * @n   - amount of elements to remove (from the end).
 * @out - store bytes into out.
 *
 * @note   - This will invalidate pointers */
TZ_DS_ARRAY_M uint32_t tz_array_popn_data(T *me, uint32_t n, void *out);

/* Get a pointer to the element[i]
 *
 * @me - array instance
 * @i - index to access */
TZ_DS_ARRAY_M void *tz_array_sub_i(T *me, uint32_t i);

/* Get a pointer to the element[i] with a typecast to t
 *
 * @me - array instance
 * @i - index to access
 * @t - type used to create the array */
#define tz_array_sub_i_t(me, i, t) ((t *)tz_array_sub_i(me, i))

/* Load element[i,i+n) into 'out[0,n)'
 *
 * @me - array instance
 * @i   - index to access
 * @out - where to place the value.
 *
 * @note- This function requires buffer and 'out' to not alias */
TZ_DS_ARRAY_M void tz_array_getn(T *me, uint32_t n, uint32_t i,
				  void *restrict out);
#define tz_array_load(me, i, out) tz_array_getn(me, 1, i, out)

/* Load 'in[0,n)' into element[i,i+n)
 *
 * @me  - array instance
 * @i   - index to access
 * @in  - where to read the value from.
 *
 * @note- This function requires buffer and 'out' to not alias */
TZ_DS_ARRAY_M void tz_array_setn(T *me, uint32_t n, uint32_t i,
				  const void *restrict in);
#define tz_array_store(me, i, in) tz_array_setn(me, 1, i, in)

/* Open a hole 'n' elements wide in the i position.
 *
 * @me - array instance
 * @n  - size of the hole to create
 * @i  - index of the first element
 * @ret - a pointer to the base of this memory.
 *
 * @note - This will invalidate pointers */
TZ_DS_ARRAY_M void *tz_array_insn(T *me, uint32_t n, uint32_t i);
#define tz_array_ins(me, i) tz_array_insn(me, 1, i)

/* Remove 'n' elements starting at 'i'.
 *
 * @me  - array instance
 * @n   - size of the hole to create
 * @i   - index of the first element
 * @ret - amount of elements actually removed.
 *
 * @note - This will invalidate pointers
 * @note - This will break sort property. */
TZ_DS_ARRAY_M uint32_t tz_array_dropn(T *me, uint32_t n, uint32_t i);
#define tz_array_drop(me, i) tz_array_dropn(me, 1, i)

/* Remove 'n' elements starting at 'i'.
 *
 * @me  - array instance
 * @n   - size of the hole to create
 * @i   - index of the first element
 * @ret - amount of elements actually removed.
 *
 * @note - This will invalidate pointers
 * @note - This will mantain sort property */
TZ_DS_ARRAY_M uint32_t tz_array_dropn_ord(T *me, uint32_t n, uint32_t i);
#define tz_array_drop_ord(me, i) tz_array_dropn_ord(me, 1, i)

/* Make it empty */
TZ_DS_ARRAY_M void tz_array_clear(T *me);

/* Swap elements [i,i+n) with element [j,j+n)
 *
 * @me  - array instance
 * @n   - amount of elements to swap
 * @i   - index of first element of block 1.
 * @j   - index of first element of block 2.
 *
 * @note - swap these pairs: (i, j), (i+1, j+1), (i+2, j+2) .. (i+n-1, j+n-1) */
TZ_DS_ARRAY_M void tz_array_swapn(T *me, uint32_t n, uint32_t i, uint32_t j);
#define tz_array_swap(me, i, j) tz_array_swapn(me, 1, i, j)

/* Swap elements [i,i+n) with element [j,j+n)
 *
 * @me  - array instance
 * @n   - amount of elements to move
 * @from- index of first element that will overwrite the [to,to+n) block.
 * @to  - index of first element that will be overwritten.
 *
 * @note - swap these pairs: (i, j), (i+1, j+1), (i+2, j+2) .. (i+n-1, j+n-1) */
TZ_DS_ARRAY_M void tz_array_moven(T *me, uint32_t n, uint32_t from,
				   uint32_t to);
#define tz_array_move(me, from, to) tz_array_moven(me, 1, from, to)

TZ_DS_ARRAY_M void tz_array_print(T *me);

#ifdef TZ_DS_ARRAY_DECLARATIONS
#define tz_array__ptr(me) ((me)->p)
#define tz_array__len(me) ((me)->n)
#define tz_array__max(me) ((me)->max)
#define tz_array__elm_size(me) ((me)->elm_size)

TZ_DS_ARRAY_M uint32_t tz_array_max_u32(uint32_t a, uint32_t b)
{
	return a >= b ? a : b;
}

TZ_DS_ARRAY_M void tz_array_init(T *me, size_t elm_size)
{
	me->p   = NULL;
	me->n   = 0;
	me->max = 0;
	me->elm_size = elm_size;
}

/* TODO: check if the compiler can do a 'Named Return Value Optimization' */
TZ_DS_ARRAY_M T tz_array_create(size_t sz)
{
	return (T){.n = 0, .max = 0, .elm_size = sz, .p = NULL};
}

TZ_DS_ARRAY_M void tz_array_destroy(T *me)
{
	free(tz_array__ptr(me));
}

TZ_DS_ARRAY_M bool tz_array_reserve(T *me, uint32_t newmax)
{
	size_t elm_size = tz_array__elm_size(me);
	void *newp = NULL;

	if (newmax < me->max)
		return true;

	if ((newp = realloc(me->p, newmax * elm_size))) {
		me->p = newp;
		me->max = newmax;
		return true;
	}
	tz_assert(newp && "ENOMEM");
	return false;
}

TZ_DS_ARRAY_M void *tz_array_base(T *me)
{
	return tz_array__ptr(me);
}

TZ_DS_ARRAY_M uint32_t tz_array_length(T *me)
{
	return tz_array__len(me);
}

TZ_DS_ARRAY_M void *tz_array_pushn(T *me, uint32_t n)
{
	uint32_t len = tz_array__len(me);
	uint32_t max = tz_array__max(me);

	// min size is 8
	if (len + n > max && !tz_array_reserve(me, tz_array_max_u32(8, 2*max + n))) {
		return NULL;
	}

	me->n += n;
	return tz_array_sub_i(me, len);
}

TZ_DS_ARRAY_M void *tz_array_pushnz(T *me, uint32_t n)
{
	void *p = tz_array_pushn(me, n);
	return memset(p, 0, n * tz_array__elm_size(me));
}

TZ_DS_ARRAY_M void *tz_array_pushn_data(T *me, uint32_t n, void *in)
{
	void *p = tz_array_pushn(me, n);
	return memcpy(p, in, n * tz_array__elm_size(me));
}

TZ_DS_ARRAY_M uint32_t tz_array_popn(T *me, uint32_t n)
{
	uint32_t len = tz_array__len(me);
	uint32_t max = tz_array__max(me);

	if (n > len) {
		n = len;
	}

	// tz_array_reserve should never fail for a shrink
	if (len - n < max / 2 &&
	    !tz_array_reserve(me, tz_array_max_u32(8, 2 * max / 3))) {
		return 0;
	}

	me->n -= n;
	return n;
}

TZ_DS_ARRAY_M uint32_t tz_array_popn_data(T *me, uint32_t n, void *out)
{
	uint32_t first = tz_array__len(me) - n;
	// check for out of bounds (underflow for unsigned).
	tz_assert(first <= tz_array__len(me) && "access out of bounds");

	void *firstp = tz_array_sub_i(me, first);
	memcpy(out, firstp, n * tz_array__elm_size(me));
	return tz_array_popn(me, n);
}

TZ_DS_ARRAY_M void *tz_array_sub_i(T *me, uint32_t i)
{
	return tz_array__ptr(me) + i * tz_array__elm_size(me);
}

TZ_DS_ARRAY_M void tz_array_getn(T *me, uint32_t n, uint32_t i,
				  void *restrict out)
{
	tz_assert(n + i <= tz_array__len(me) && "access out of bounds");
	memcpy(out, tz_array_sub_i(me, i), n * tz_array__elm_size(me));
}

TZ_DS_ARRAY_M void tz_array_setn(T *me, uint32_t n, uint32_t i,
				  const void *restrict in)
{
	tz_assert(n + i <= tz_array__len(me) && "access out of bounds");
	memcpy(tz_array_sub_i(me, i), in, n * tz_array__elm_size(me));
}

TZ_DS_ARRAY_M void *tz_array_insn(T *me, uint32_t n, uint32_t i)
{
	// TODO: add assert
	/* (let @ be the changing indices)
	 * ######@@@@     : original array
	 *       xxx      : i,i+n
	 * ######@@@@---  : aloc n at the end.
	 * ######---@@@@  : move len-(i+n) elms from i to i+n.
	 *       ^        : return &buf[i] */
	tz_array_pushn(me, n);
	uint32_t len = tz_array__len(me);
	tz_array_moven(me, len - (i + n), i, i + n);
	return tz_array_sub_i(me, i);
}

TZ_DS_ARRAY_M uint32_t tz_array_dropn(T *me, uint32_t n, uint32_t i)
{
	// TODO: add assert
	/* (let @ be the changing indices)
	 * case 1:
	 * #######@@@     : original array
	 *    xxx         : i,i+n
	 * ###@@@#        : move n from len-n to i
	 *
	 * case 2:
	 * #########@     : original array
	 *       xxx      : i,i+n
	 * ######@        : move len-(i+n) from len-(i+n) to i
	 *
	 * case 3:
	 * @              : original array
	 *                : move 0 from len-n to i */
	uint32_t len = tz_array__len(me);
	tz_assert(len >= (i + n) && "out of bounds access");

	uint32_t m = len - (i + n) < n ? len - (i + n) : n;
	tz_array_moven(me, m, len - m, i);
	tz_array_popn(me, n);
	return m;
}

TZ_DS_ARRAY_M uint32_t tz_array_dropn_ord(T *me, uint32_t n, uint32_t i)
{
	// TODO: add assert
	/*
	 * #####---@@     : original array
	 *      xxx       : i,i+n
	 * #####@@---     : move len-(i+n) elms from i+n to i
	 * #####@@        : then pop */
	uint32_t len = tz_array__len(me);
	tz_array_moven(me, len - (i + n), i + n, i);
	return tz_array_popn(me, n);
}

TZ_DS_ARRAY_M void tz_array_clear(T *me)
{
	tz_array__len(me) = 0;
}

TZ_DS_ARRAY_M void tz_array_swapn(T *me, uint32_t n, uint32_t i, uint32_t j)
{
	uint32_t len = tz_array__len(me);
	tz_assert(i + n <= len && j + n <= len && "access out of bounds");

	uint8_t tmp[n * tz_array__elm_size(me)];
	tz_array_getn(me, n, j, tmp);
	tz_array_moven(me, n, i, j);
	tz_array_setn(me, n, i, tmp);
}

TZ_DS_ARRAY_M void tz_array_moven(T *me, uint32_t n, uint32_t from,
				   uint32_t to)
{
	uint32_t len = tz_array__len(me);
	tz_assert(tz_array_max_u32(from, to) + n <= len && "access out of bounds");

	void *from_ = tz_array_sub_i(me, from);
	void *to_ = tz_array_sub_i(me, to);
	memmove(to_, from_, n * tz_array__elm_size(me));
}

TZ_DS_ARRAY_M T tz_array_build(void *p, uint32_t n, size_t elm_size)
{
	return (T){.p = (void *)p, .n = 4, .max = 4, .elm_size = elm_size};
}

TZ_DS_ARRAY_M void tz_array_print(T *me)
{
	printf("(%d/%d, %ld)", me->n, me->max, me->elm_size);
}

/** @} */
#endif /* TZ_DS_ARRAY_DECLARATIONS */
#undef T
#endif /* TZ_DS_ARRAY_H */
