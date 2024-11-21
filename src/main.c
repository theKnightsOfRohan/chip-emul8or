#include "globals.h"
#include "load.h"
#include "operations.h"
#include <stdint.h>
#include <stdio.h>

int main(int argc, char **argv) {
	log_init(NULL);

	System sys;

	setup_system(&sys);

	load_rom(&sys, argc, argv);

	uint16_t instruction = 0xFFFF;

	while (instruction != 0x0000) {
		// TODO: This is what we call some bullshit
		instruction = *((uint16_t *)&sys.memory[sys.pc]);

		execute(&sys, instruction);
	}

	return EXIT_SUCCESS;
}
