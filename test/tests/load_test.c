#include "load_test.h"
#include "test_utils.h"
#include <stdint.h>

bool load_rom_test(System *sys) {
	bool pass = true;
	uint8_t contents[4] = {0x48, 0x00, 0x49, 0x00};

	load_rom(sys, "./test/roms/test/load_test.ch8");

	pass = memassert(sys->memory, contents, 0x0200, 4);

	return pass;
}

int load_test(System *sys) {
	int tests_run = 0, tests_passed = 0;
	Log("=======> LOAD TESTS\n");

	setup_system(sys);

	tests_run++;
	tests_passed += load_rom_test(sys);

	Log("LOAD: passed %d/%d <=======\n", tests_passed, tests_run);

	return tests_passed;
}
