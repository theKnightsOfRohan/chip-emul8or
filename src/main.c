#include "globals.h"
#include "load.h"
#include "operations.h"
#include <stdint.h>
#include <stdio.h>

int main(int argc, char **argv) {
	log_init();

	System sys;

	setup_system(&sys);

	assert(argc == 2);

	char *file_path = argv[1];

	load_rom(&sys, file_path);

	uint16_t instruction = 0xFFFF;

	while (instruction != 0x0000) {
		// TODO: This is what we call some bullshit
		instruction = *((uint16_t *)&sys.memory[sys.pc]);

		// Make sure to swap from big endian to make parsing easier
		instruction =
			((instruction & 0xFF00) >> 8) | ((instruction & 0x00FF) << 8);
		execute(&sys, instruction);
	}

	return EXIT_SUCCESS;
}
