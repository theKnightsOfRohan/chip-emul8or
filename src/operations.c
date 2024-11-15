#include "operations.h"
#include "globals.h"
#include <stdint.h>

void clear_or_ret(System *sys, uint16_t args) {}

void execute(System *sys, uint16_t operation) {
	int opcode = (operation & 0xF000) >> 12;
	int args = (operation & 0x0FFF);

	Log(1, "EXECUTE: Opcode: %x, args: %x\n", opcode, args);

	sys->program_counter += 1;

	instruction_set[0] = &clear_or_ret;
}
