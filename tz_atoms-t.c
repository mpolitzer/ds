#include <string.h>

#define TZ_UNIT_TEST_INLINE
#include "tz_unit_test.h"

#define TZ_DS_ATOMS_INLINE
#include "tz_atoms.h"

void test_smoke()
{
	tz_atoms as = tz_atoms_create();

	tz_atom a = tz_atoms_put(&as, "hello");
	tz_atom b = tz_atoms_put(&as, "hello");
	tz_atom c = tz_atoms_put(&as, "world");
	tz_atom d = tz_atoms_put(&as, "world");

	tz_unit_test_should_pass(tz_atom_eq(a, b) && tz_atom_eq(c, d));
	tz_unit_test_should_fail(tz_atom_eq(a, c) || tz_atom_eq(b, d));

	tz_unit_test_should_pass(
			strcmp( tz_atoms_get(&as, a),
				tz_atoms_get(&as, b)) == 0);

	tz_atoms_destroy(&as);
}

int main(int argc, char const* argv[])
{
	tz_unit_test_start(argv[0]);
	test_smoke();
	return tz_unit_test_report();
}
