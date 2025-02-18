#include "test_utils.h"
#include <stdint.h>
#include <string.h>

// TODO: Stubs written. Tests unneeded for simpler ops.

bool syscall_test(System *sys) {
	bool pass = true;

	// TEST: Clear display
	uint8_t expected[32][8] = {{0}};

	for (int i = 0; i < 32; i++) {
		memset(sys->_display[i], 1, 8);
	}

	// syscall(clear_display)
	execute(sys, 0x00E0);

	for (int i = 0; i < 32; i++) {
		pass = memassert(sys->_display[i], expected[i], 0, 8);

		if (!pass) {
			Log("CLEAR_DISPLAY: Unexpected value");
			break;
		}
	}

	// TEST: Return from subroutine
	setup_system(sys);

	// Act as if in subroutine
	sys->stack_ptr += 1;
	uint16_t junk[2] = {2, 1};

	memcpy(sys->call_stack, junk, 2);

	// syscall(return)
	execute(sys, 0x00EE);

	if (sys->pc != 2)
		Log("RET: PC not set\n");

	if (sys->stack_ptr != 0)
		Log("RET: stack ptr not decremented\n");

	return pass;
}

bool jump_test(System *sys) {
	bool pass = true;
	return pass;
}

bool call_test(System *sys) {
	bool pass = true;
	return pass;
}

bool skip_reg_equal_val_test(System *sys) {
	bool pass = true;
	return pass;
}

bool skip_reg_not_equal_val_test(System *sys) {
	bool pass = true;
	return pass;
}

bool skip_regs_equal_test(System *sys) {
	bool pass = true;
	return pass;
}

bool reg_set_val_test(System *sys) {
	bool pass = true;
	return pass;
}

bool reg_add_val_test(System *sys) {
	bool pass = true;
	return pass;
}

bool math_test(System *sys) {
	bool pass = true;
	return pass;
}

bool skip_regs_not_equal_test(System *sys) {
	bool pass = true;
	return pass;
}

bool store_addr_test(System *sys) {
	bool pass = true;
	return pass;
}

bool jump_offset_test(System *sys) {
	bool pass = true;
	return pass;
}

bool bitwise_rand_test(System *sys) {
	bool pass = true;
	return pass;
}

bool draw_sprite_test(System *sys) {
	bool pass = true;
	return pass;
}

bool skip_key_test(System *sys) {
	bool pass = true;
	return pass;
}

bool sys_ops_test(System *sys) {
	bool pass = true;
	return pass;
}

bool (*tests[16])(System *sys) = {
	&syscall_test,
	&jump_test,
	&call_test,
	&skip_reg_equal_val_test,
	&skip_reg_not_equal_val_test,
	&skip_regs_equal_test,
	&reg_set_val_test,
	&reg_add_val_test,
	&math_test,
	&skip_regs_not_equal_test,
	&store_addr_test,
	&jump_offset_test,
	&bitwise_rand_test,
	&draw_sprite_test,
	&skip_key_test,
	&sys_ops_test,
};

int operations_test(System *sys) {
	Log("=======> OPERATIONS TEST\n");

	int tests_run = 0, tests_passed = 0;

	for (int i = 0; i < 16; i++) {
		setup_system(sys);
		tests_run++;
		tests_passed += tests[i](sys);
	}

	Log("OPERATIONS: passed %d/%d <=======\n", tests_passed, tests_run);

	return tests_passed;
}
