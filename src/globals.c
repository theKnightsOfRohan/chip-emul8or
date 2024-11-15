#include "globals.h"
#include <assert.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void safe_exit() { exit(0); }

void log_init(char *log_file) {
#ifndef NO_LOG

	if (log_file == NULL) {
		log_file = "log.txt";
	}

	assert(snprintf(LOG_FILE, sizeof(LOG_FILE), "%s%s", "log/", log_file) > 0);

	LOG_FILE_HANDLE = fopen(LOG_FILE, "a");
	eassert(LOG_FILE_HANDLE != NULL);

	Log(1, "=======> LOG_INIT\n");

	atexit(log_close);

	signal(SIGINT, safe_exit);
	signal(SIGSEGV, safe_exit);

#endif
}

void log_close() {
#ifndef NO_LOG
	Log(1, "LOG_END <=======\n");

	fclose(LOG_FILE_HANDLE);

#endif
}

void add_pc(System *sys, uint8_t val) { sys->pc += (val * 2); }

void setup_system(System *sys) {
	System val = {
		{0}, {0},	0,		 {0},
		0,	 0x200, &add_pc, {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}}};

	*sys = val;
}

void *smalloc(size_t size) {
	void *ret = malloc(size);
	eassert(ret != NULL);
	memset(ret, 0, size);
	return ret;
}
