#define TZ_UNIT_TEST_INLINE
#include "tz_unit_test.h"

#define TZ_DS_STRING_INLINE
#include "tz_string.h"

void check_format()
{
	tz_string s = tz_string_create();
	tz_string_format(&s, "hello world:%d %d", 1234, 1234);

	char expected[256];
	snprintf(expected, sizeof expected, "hello world:%d %d", 1234, 1234);

	tz_unit_test_should_pass(strncmp(expected, (char *)s.p, s.n) == 0);
}

void check_toupper()
{
	char *input    = "hello world";
	char *expected = "HELLO WORLD";

	tz_string s = tz_string_create();
	tz_string_toupper_s(&s, input);

	tz_unit_test_should_pass(strncmp(expected, (char *)s.p, s.n) == 0);
}

void check_split()
{
	char *input       = " hello world ";
	char *expected[2] = {
		"hello",
		"world"
	};

	tz_array strings = tz_string_split(input);
	tz_string *s0 = tz_array_sub_i(&strings, 0);
	tz_string *s1 = tz_array_sub_i(&strings, 1);

	//printf("1.|"TZ_STRING_TO_FMT"|\n", tz_string_to_fmt(s0));
	//printf("2.|"TZ_STRING_TO_FMT"|\n", tz_string_to_fmt(s1));
	tz_unit_test_should_pass(
			memcmp(s0->p, expected[0], s0->n) == 0 &&
			memcmp(s1->p, expected[1], s1->n) == 0);
}

int main(int argc, char const* argv[])
{
	tz_unit_test_start(argv[0]);
	check_format();
	check_toupper();
	check_split();
//	check_capitalize();
	return tz_unit_test_report();
}
