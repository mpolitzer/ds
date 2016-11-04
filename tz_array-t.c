#define TZ_UNIT_TEST_INLINE
#include "tz_unit_test.h"

#define TZ_DS_ARRAY_INLINE
#include "tz_array.h"

static void test_indexation(void) {
	bool eq=true;
	int is[4] = { 0, 1, 2, 3 };
	struct tz_array_t a = tz_array_build(is, 4, sizeof(int));

	eq &= memcmp(tz_array_sub_i(&a, 0), &is[0], sizeof(int)) == 0;
	eq &= memcmp(tz_array_sub_i(&a, 1), &is[1], sizeof(int)) == 0;
	eq &= memcmp(tz_array_sub_i(&a, 2), &is[2], sizeof(int)) == 0;
	eq &= memcmp(tz_array_sub_i(&a, 3), &is[3], sizeof(int)) == 0;

	tz_unit_test_should_pass(eq);
}

static void test_push1(void) {
	bool eq=true;
	struct tz_array_t a = tz_array_create(sizeof(int));

	for (int i=0; i<100; ++i)
		*(int *)tz_array_pushn(&a, 1) = i;

	for (int i=0; i<100; ++i)
		eq &= memcmp(tz_array_sub_i(&a, i), &i, sizeof(int)) == 0;

	tz_unit_test_should_pass(eq);
	tz_array_destroy(&a);
}

static void test_pop1(void) {
	bool eq=true;
	struct tz_array_t a = tz_array_create(sizeof(int));

	int *p = (int *)tz_array_pushn(&a, 100);
	for (int i=0; i<100; ++i)
		*p = i;

	for (int i=tz_array_length(&a)-1; i; i = i-tz_array_popn(&a, 1))
		eq &= memcmp(tz_array_sub_i(&a, i), &i, sizeof(int)) == 0;

	tz_unit_test_should_pass(eq);
	tz_array_destroy(&a);
}

static void test_get(void) {
	bool eq;
	int is[4] = { 2, 3, 5, 7 };
	struct tz_array_t a = tz_array_build(is, 4, sizeof(int));

	{ // check if i == 0 works
		int os[4] = { 0, 0, 0, 0 };
		eq=true;
		tz_array_getn(&a, 4, 0, &os[0]);
		eq &= os[0] == is[0];
		eq &= os[1] == is[1];
		eq &= os[2] == is[2];
		eq &= os[3] == is[3];

		tz_unit_test_should_pass(eq);
	}

	{ // check if i == 1 works
		int os[4] = { 0, 0, 0, 0 };
		eq=true;
		tz_array_getn(&a, 3, 1, os);
		eq &= os[0] == is[1];
		eq &= os[1] == is[2];
		eq &= os[2] == is[3];
		eq &= os[3] == 0;

		tz_unit_test_should_pass(eq);
	}

	{ // check if i == 2 works
		int os[4] = { 0, 0, 0, 0 };
		eq=true;
		tz_array_getn(&a, 2, 2, os);
		eq &= os[0] == is[2];
		eq &= os[1] == is[3];
		eq &= os[2] == 0;
		eq &= os[3] == 0;

		tz_unit_test_should_pass(eq);
	}

	{ // check if i == 3 works
		int os[4] = { 0, 0, 0, 0 };
		eq=true;
		tz_array_getn(&a, 1, 3, os);
		eq &= os[0] == is[3];
		eq &= os[1] == 0;
		eq &= os[2] == 0;
		eq &= os[3] == 0;

		tz_unit_test_should_pass(eq);
	}

	{ // check if n == 0 works
		int os[4] = { 0, 0, 0, 0 };
		eq=true;
		tz_array_getn(&a, 0, 4, os);
		eq &= os[0] == 0;
		eq &= os[1] == 0;
		eq &= os[2] == 0;
		eq &= os[3] == 0;

		tz_unit_test_should_pass(eq);
	}
}

static void test_set(void) {
	{ // check if i == 0 works
		int os[4] = { 0, 0, 0, 0 };
		struct tz_array_t a = tz_array_build(os, 4, sizeof(int));
		int *p, is[4] = { 2, 3, 5, 7 };
		bool eq=true;
		tz_array_setn(&a, 4, 0, &is[0]);
		p = tz_array_base(&a);
		eq &= p[0] == is[0];
		eq &= p[1] == is[1];
		eq &= p[2] == is[2];
		eq &= p[3] == is[3];

		tz_unit_test_should_pass(eq);
	}
	{ // check if i == 1 works
		int os[4] = { 0, 0, 0, 0 };
		struct tz_array_t a = tz_array_build(os, 4, sizeof(int));
		int *p, is[4] = { 2, 3, 5, 7 };
		bool eq=true;
		tz_array_setn(&a, 3, 1, &is[0]);
		p = tz_array_base(&a);
		eq &= p[0] == 0;
		eq &= p[1] == is[0];
		eq &= p[2] == is[1];
		eq &= p[3] == is[2];

		tz_unit_test_should_pass(eq);
	}
	{ // check if i == 2 works
		int os[4] = { 0, 0, 0, 0 };
		struct tz_array_t a = tz_array_build(os, 4, sizeof(int));
		int *p, is[4] = { 2, 3, 5, 7 };
		bool eq=true;
		tz_array_setn(&a, 2, 2, &is[0]);
		p = tz_array_base(&a);
		eq &= p[0] == 0;
		eq &= p[1] == 0;
		eq &= p[2] == is[0];
		eq &= p[3] == is[1];

		tz_unit_test_should_pass(eq);
	}
	{ // check if i == 3 works
		int os[4] = { 0, 0, 0, 0 };
		struct tz_array_t a = tz_array_build(os, 4, sizeof(int));
		int *p, is[4] = { 2, 3, 5, 7 };
		bool eq=true;
		tz_array_setn(&a, 1, 3, &is[0]);
		p = tz_array_base(&a);
		eq &= p[0] == 0;
		eq &= p[1] == 0;
		eq &= p[2] == 0;
		eq &= p[3] == is[0];

		tz_unit_test_should_pass(eq);
	}
	{ // check if n == 0 works
		int os[4] = { 0, 0, 0, 0 };
		struct tz_array_t a = tz_array_build(os, 4, sizeof(int));
		int *p, is[4] = { 2, 3, 5, 7 };
		bool eq=true;
		tz_array_setn(&a, 0, 4, &is[0]);
		p = tz_array_base(&a);
		eq &= p[0] == 0;
		eq &= p[1] == 0;
		eq &= p[2] == 0;
		eq &= p[3] == 0;

		tz_unit_test_should_pass(eq);
	}
}

static void test_move(void) {
	bool eq=true;
	struct tz_array_t a = tz_array_create(sizeof(int));
	int *p = tz_array_pushn(&a, 10);

	for (int i=0; i<10; ++i) {
		tz_array_moven(&a, 9, 0, 1);
		p[0] = i;
	}

	for (int i=0; i<10; ++i)
		eq &= *(int *)tz_array_sub_i(&a, i) == 9-i;

	tz_unit_test_should_pass(eq);
	tz_array_destroy(&a);
}

static void test_ins1(void) {
	bool eq=true;
	struct tz_array_t a = tz_array_create(sizeof(int));

	for (int i=0; i<10; ++i)
		*(int *)tz_array_insn(&a, 1, 0) = i;

	for (int i=0; i<10; ++i)
		eq &= *(int *)tz_array_sub_i(&a, i) == 9-i;

	tz_unit_test_should_pass(eq);
	tz_array_destroy(&a);
}

static void test_ins2(void) {
	bool eq=true;
	struct tz_array_t a = tz_array_create(sizeof(int));

	for (int i=0; i<10; i += 2) {
		int *p = tz_array_insn(&a, 2, 0);
		p[0] = i+1;
		p[1] = i;
	}

	for (int i=0; i<10; ++i)
		eq &= *(int *)tz_array_sub_i(&a, i) == 9-i;

	tz_unit_test_should_pass(eq);
	tz_array_destroy(&a);
}

static void test_drop(void) {
	//bool eq=true;
	struct tz_array_t a = tz_array_create(sizeof(int));
	int *p = tz_array_pushn(&a, 10);
	for (int i=0; i<10; i++)
		p[i] = i;

	do {
		//print_as_int(&a);
	} while (tz_array_dropn(&a, 2, 0) >= 2);

	//tz_unit_test_should_pass(eq);
	tz_array_destroy(&a);
}

static void test_drop_ord(void) {
	bool eq=true;
	struct tz_array_t a = tz_array_create(sizeof(int));
	int *p = tz_array_pushn(&a, 10);
	for (int i=0; i<10; i++)
		p[i] = i;

	tz_unit_test_should_pass(eq);
	tz_array_destroy(&a);
}

static void test_swap(void) {
	bool eq=true;
	int is[4] = { 0, 1, 2, 3 };
	int os[4] = { 3, 2, 1, 0 };
	struct tz_array_t a = tz_array_build(is, 4, sizeof(int));

	tz_array_swapn(&a, 2, 0, 2);
	tz_array_swapn(&a, 1, 0, 1);
	tz_array_swapn(&a, 1, 2, 3);

	eq &= memcmp(tz_array_base(&a), os, sizeof(os)) == 0;
	tz_unit_test_should_pass(eq);
}

static void test_clear(void) {
	int is[4] = { 0, 1, 2, 3 };
	struct tz_array_t a = tz_array_build(is, 4, sizeof(int));

	tz_array_clear(&a);
	tz_unit_test_should_pass(tz_array_length(&a) == 0);
}

int main(int argc, const char *argv[])
{
	tz_unit_test_start(argv[0]);

	test_indexation();
	test_move();
	test_swap();
	test_push1();
	test_pop1();
	test_get();
	test_set();
	test_ins1();
	test_ins2();
	test_drop();
	test_drop_ord();
	test_clear();

	return tz_unit_test_report();
}
