#include "load.h"
#include "globals.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void load_rom(System *sys, char *file_path) {
	FILE *rom_file;

	Log("LOAD: Opening %s\n", file_path);

	eassert((rom_file = fopen(file_path, "rb")) != NULL);

	int rom_fd = fileno(rom_file);

	size_t length;
	eassert((length = lseek(rom_fd, 0L, SEEK_END)) != 0);
	assert(length % 2 == 0); // 16-bit instructions means multiple of 2 bytes
	rewind(rom_file);

	Log("LOAD: Length of %s is %ld\n", file_path, length);

	uint8_t *contents = smalloc(length);

	fread(contents, 1, length, rom_file);
	eassert(feof(rom_file) == 0); // Up until end, don't try to read the end

	assert(sizeof(char) == sizeof(uint8_t));
	memcpy((sys->memory + 0x200), contents, length);

	// Log("LOAD: Contents:\n%s\n", (char *)(sys->memory + 0x200));

	fclose(rom_file);
}
