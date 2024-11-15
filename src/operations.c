#include "operations.h"
#include "globals.h"
#include <stdint.h>
#include <string.h>

void syscall(System *sys, uint16_t args) {
	assert((args & 0xF000) == 0x0000);

	uint16_t arg = args & 0x000F;

	if (arg == 0x0000) {
		for (int i = 0; i < 8; i++) {
			memset(sys->display[i], 0, 4);
		}
	} else if (arg == 0x000E) {
		assert(sys->stack_ptr >= 0);

		sys->pc = sys->call_stack[sys->pc];

		sys->stack_ptr -= 1;
	}
}

void jump(System *sys, uint16_t args) {
	assert((args & 0xF000) == 0x1000);

	sys->pc = args & 0x0FFF;
}

void call(System *sys, uint16_t args) {
	assert((args & 0xF000) == 0x2000);

	sys->stack_ptr += 1;

	sys->call_stack[sys->stack_ptr] = sys->pc;
	sys->pc = args & 0x0FFF;
}

void skip_equal(System *sys, uint16_t args) {
	assert((args & 0xF000) == 0x3000);

	uint16_t reg = (args & 0x0F00) >> 8;
	uint8_t val = args & 0x00FF;

	if (sys->registers[reg] == val)
		sys->add_pc(sys, 1);
}

instruction instruction_set[35] = {
	&syscall,
	&jump,
	&call,
	&skip_equal,
};

void execute(System *sys, uint16_t operation) {
	uint16_t opcode = (operation & 0xF000) >> 12;
	uint16_t args = (operation & 0x0FFF);

	Log(1, "EXECUTE: Opcode: %x, args: %03x\n", opcode, args);

	sys->add_pc(sys, 1);

	instruction_set[opcode](sys, args);
}
