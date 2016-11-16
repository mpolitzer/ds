#define TZ_UNIT_TEST_INLINE
#include "tz_unit_test.h"

#define TZ_DS_OBJS_INLINE
#include "tz_objs.h"

#define SIZE(x) sizeof(x)/sizeof(*x)

// TODO: requires instrumentation to test
//static void force_obj_reuse()
//{
//	tz_obj objs[2048];
//	tz_objs os = tz_objs_create();
//
//	for (int i=0; i<SIZE(objs); ++i){
//		tz_obj obj = tz_objs_new_obj(&os);
//		tz_objs_del_obj(&os, obj);
//	}
//
//	for (int i=0; i<SIZE(objs); ++i) {
//		objs[i] = tz_objs_new_obj(&os);
//	}
//}

static void test_smoke()
{
	bool eq;

	tz_obj  o[2*1024]; // must be even
	tz_objs os = tz_objs_create();

	{	// test if new + isalive works.
		eq = true;
		for (int i=0; i<SIZE(o); ++i)
			o[i] = tz_objs_new_obj(&os);

		for (int i=0; i<SIZE(o); ++i)
			eq &= tz_objs_obj_isalive(&os, o[i]);

		tz_unit_test_should_pass(eq);
	}

	{	// test if delete + isalive works.
		eq = true;
		for (int i=0; i<SIZE(o); i += 2)
			tz_objs_del_obj(&os, o[i]);

		for (int i=0; i<SIZE(o); i += 2){
			eq &= tz_objs_obj_isalive(&os, o[i])   == false;
			eq &= tz_objs_obj_isalive(&os, o[i+1]) == true;
		}
		tz_unit_test_should_pass(eq);
	}

}

int main(int argc, const char *argv[])
{
	tz_unit_test_start(argv[0]);
	test_smoke();
	return tz_unit_test_report();
}
