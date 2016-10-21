#define TZ_UNIT_TEST_INLINE
#include "tz_unit_test.h"

#define TZ_STD_STRING_INLINE
#include "tz_string.h"

int main(int argc, char const* argv[])
{
	tz_string s = tz_string_create();
	tz_string_format(&s, "hello world:%d %d", 1234, 1234);

	char buf[256];
	uint32_t n = snprintf(buf, sizeof buf, "hello world:%d %d", 1234, 1234);

	tz_unit_test_start(argv[0]);
	tz_unit_test_should_pass(strncmp(buf, (char *)s.p, s.n) == 0);
	return tz_unit_test_report();
}
