#define TZ_UNIT_TEST_INLINE
#include "tz_unit_test.h"

int main(int argc, char const* argv[])
{
	tz_unit_test_start(argv[0]);
	tz_unit_test_should_pass(true);
	tz_unit_test_should_fail(false);
	return tz_unit_test_report();
}
