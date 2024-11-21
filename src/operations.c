#include "operations.h"
#include "globals.h"
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

void syscall(System *sys, uint16_t args) {
	assert((args & 0xF000) == 0x0000);

	uint16_t op = args & 0x00FF;

	if ((op & 0x00F0) == 0x00C0) { // TODO: Scroll display down
		uint8_t scroll_amt = op & 0x000F;
	} else {
		switch (op) {
		case 0x00E0: // NOTE: Clear display
			for (int i = 0; i < 32; i++) {
				memset(sys->_display[i], 0, 8);
			}
		case 0x00EE: // NOTE: Return from subroutine
			assert(sys->stack_ptr >= 0);

			sys->pc = sys->call_stack[sys->pc];

			sys->stack_ptr -= 1;

		case 0x00FB: // TODO: Scroll display 4 pixels left
			noop();
		case 0x00FC: // TODO: Scroll display 4 pixels right
			noop();
		case 0x00FD: // TODO: Exit program
			noop();
		case 0x00FE: // TODO: Disable extended screen mode
			noop();
		case 0x00FF: // TODO: Enable extended screen mode
			noop();
		default:
			Log(1, "COMMAND MATH: Unrecognized operation %02X\n", op);
			// TODO: Assert state and error flag in sys
		}
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

void skip_reg_equal_val(System *sys, uint16_t args) {
	assert((args & 0xF000) == 0x3000);

	uint16_t reg = (args & 0x0F00) >> 8;
	uint8_t val = args & 0x00FF;

	if (sys->registers[reg] == val)
		sys->add_pc(sys, 1);
}

void skip_reg_not_equal_val(System *sys, uint16_t args) {
	assert((args & 0xF000) == 0x4000);

	uint16_t reg = (args & 0x0F00) >> 8;
	uint8_t val = args & 0x00FF;

	if (sys->registers[reg] != val)
		sys->add_pc(sys, 1);
}

void skip_regs_equal(System *sys, uint16_t args) {
	assert((args & 0xF000) == 0x5000);

	uint16_t reg1 = (args & 0x0F00) >> 8;
	uint16_t reg2 = (args & 0x00F0) >> 4;
	assert((args & 0x000F) == 0x0000);

	if (sys->registers[reg1] == sys->registers[reg2])
		sys->add_pc(sys, 1);
}

void reg_set_val(System *sys, uint16_t args) {
	assert((args & 0xF000) == 0x6000);

	uint16_t reg = (args & 0x0F00) >> 8;
	uint8_t val = (args & 0x00FF);

	sys->registers[reg] = val;
}

void reg_add_val(System *sys, uint16_t args) {
	assert((args & 0xF000) == 0x7000);

	uint16_t reg = (args & 0x0F00) >> 8;
	uint8_t val = args & 0x00FF;

	sys->registers[reg] += val;
}

void math(System *sys, uint16_t args) {
	assert((args & 0xF000) == 0x8000);

	uint8_t op = args & 0x000F;
	uint8_t dest = (args & 0x0F00) >> 8;
	uint8_t src = (args & 0x00F0) >> 4;

	bool set_flag = false;
	uint8_t flag = 0;

	switch (op) {
	case 0x00:
		sys->registers[dest] = sys->registers[src];
	case 0x01:
		sys->registers[dest] |= sys->registers[src];
	case 0x02:
		sys->registers[dest] &= sys->registers[src];
	case 0x03:
		sys->registers[dest] ^= sys->registers[src];
	case 0x04:
		sys->registers[dest] += sys->registers[src];
		set_flag = sys->registers[dest] < sys->registers[src];
		flag = set_flag;
	case 0x05:
		sys->registers[dest] = sys->registers[dest] - sys->registers[src];
		set_flag = sys->registers[dest] >= sys->registers[src];
		flag = set_flag;
	case 0x06:
		set_flag = true;
		flag = sys->registers[dest] & 0x01;
		sys->registers[dest] >>= 1;
	case 0x07:
		sys->registers[dest] = sys->registers[src] - sys->registers[dest];
		set_flag = sys->registers[src] >= sys->registers[dest];
		flag = set_flag;
	case 0x0E:
		set_flag = true;
		flag = (sys->registers[dest] & 0x80) >> 7;
		sys->registers[dest] <<= 1;
	default:
		Log(1, "COMMAND MATH: Unrecognized operation %02X\n", op);
		// TODO: Assert state and error flag in sys
	}

	if (set_flag)
		sys->registers[0xF] = flag;
}

void skip_regs_not_equal(System *sys, uint16_t args) {
	assert((args & 0xF00F) == 0x9000);

	uint8_t reg1 = (args & 0x0F00) >> 8;
	uint8_t reg2 = (args & 0x00F0) >> 4;

	if (sys->registers[reg1] == sys->registers[reg2])
		sys->add_pc(sys, 1);
}

void store_addr(System *sys, uint16_t args) {
	assert((args & 0xF000) == 0xA000);

	sys->addr_reg = args & 0x0FFF;
}

void jump_offset(System *sys, uint16_t args) {
	assert((args & 0xF000) == 0xB000);

	sys->pc = sys->registers[0x0] + (args & 0x0FFF);
}

void bitwise_rand(System *sys, uint16_t args) {
	assert((args & 0xF000) == 0xC000);

	uint8_t reg = (args & 0x0F00) >> 8;
	uint8_t val = rand();

	sys->registers[reg] = val & (args & 0x00FF);
}

void draw_sprite(System *sys, uint16_t args) {
	assert((args & 0xF000) == 0xD000);

	// TODO: Implement draw sprite to screen
}

void skip_key(System *sys, uint16_t args) {
	assert((args & 0xF000) == 0xE000);

	uint8_t reg = (args & 0x0F00) >> 8;
	uint8_t op = args & 0x00FF;

	bool is_key;

	// TODO: Ncurses deez nuts
	switch (op) {
	case 0x9E: // TODO: Key is pressed
	case 0xA1: // TODO: Key not pressed
	default:
		Log(1, "COMMAND SKIP_KEY: Unrecognized operation %02X\n", op);
		// TODO: Assert state and error flag in sys
	}
}

void sys_ops(System *sys, uint16_t args) {
	assert((args & 0xF000) == 0xF000);

	uint8_t op = args & 0x00FF;
	uint8_t reg = (args & 0x0F00) >> 8;

	switch (op) {
	case 0x07:
		sys->registers[reg] = sys->read_delay_timer(sys);
	case 0x0A: // TODO: Block until key is pressed, then store into register
	case 0x15:
		sys->set_delay_timer(sys, sys->registers[reg]);
	case 0x18:
		sys->set_sound_timer(sys, sys->registers[reg]);
	case 0x1E:
		sys->addr_reg += sys->registers[reg];
	case 0x29:
		// TODO: Set value of address register to location of 5-byte sprite
		// given by Vx
	case 0x30:
		// TODO: Set value of address register to location of 10-byte sprite
		// given by Vx
	case 0x33: // Store binary-coded decimal representation of register at I,
			   // I+1, I+2
		sys->memory[sys->addr_reg] = sys->registers[reg] / 100;
		sys->memory[sys->addr_reg + 1] = (sys->registers[reg] % 100) / 10;
		sys->memory[sys->addr_reg + 2] = sys->registers[reg] % 10;
	case 0x55: // Dump V0:Vx (inclusive) in memory I0:Ix
		for (int i = 0; i <= reg; i++) {
			sys->memory[sys->addr_reg + i] = sys->registers[i];
		}
	case 0x65: // Dump I0:Ix (inclusive) in registers V0:Vx
		for (int i = 0; i <= reg; i++) {
			sys->registers[i] = sys->memory[sys->addr_reg + i];
		}
	case 0x75: // NOTE: Calculator-specific instructions, shouldn't need
	case 0x85:
	default:
		Log(1, "COMMAND SYS_OPS: Unrecognized operation %02X\n", op);
		// TODO: Assert state and error flag in sys
	}
}

instruction instruction_set[16] = {
	&syscall,
	&jump,
	&call,
	&skip_reg_equal_val,
	&skip_reg_not_equal_val,
	&skip_regs_equal,
	&reg_set_val,
	&reg_add_val,
	&math,
	&skip_regs_not_equal,
	&store_addr,
	&jump_offset,
	&bitwise_rand,
	&draw_sprite,
	&skip_key,
	&sys_ops,
};

void execute(System *sys, uint16_t operation) {
	uint16_t opcode = (operation & 0xF000) >> 12;
	uint16_t args = (operation & 0x0FFF);

	Log(1, "EXECUTE: Opcode: %X, args: %03X\n", opcode, args);

	sys->add_pc(sys, 1);

	// instruction_set[opcode](sys, operation);
}
