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

	// TODO: This is what we call some bullshit
	uint16_t instruction = *((uint16_t *)&sys.memory[sys.pc]);

	execute(&sys, instruction);

	return EXIT_SUCCESS;
}
