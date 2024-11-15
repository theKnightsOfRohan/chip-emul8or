#include "globals.h"
#include "load.h"
#include "operations.h"
#include <stdint.h>
#include <stdio.h>

int main(int argc, char **argv) {
	log_init(NULL);

	System sys = {
		{0}, {0}, 0, {0}, 0, 0x200, {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}}};

	load_rom(&sys, argc, argv);

	execute(&sys, sys.memory[sys.program_counter]);

	return EXIT_SUCCESS;
}
