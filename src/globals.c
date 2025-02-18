#include "globals.h"
#include <assert.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void safe_exit() { exit(0); }

void log_init() {
#ifndef NO_LOG

#ifndef LOG_FILE_NAME
	assert(snprintf(LOG_FILE, sizeof(LOG_FILE), "log/log.txt") > 0);
#else
	assert(snprintf(LOG_FILE, sizeof(LOG_FILE), "log/%s", LOG_FILE_NAME) > 0);
#endif

	LOG_FILE_HANDLE = fopen(LOG_FILE, "a");
	eassert(LOG_FILE_HANDLE != NULL);

	Log("=======> LOG_INIT\n");

	atexit(log_close);

	signal(SIGINT, safe_exit);
	signal(SIGSEGV, safe_exit);

#endif
}

void log_close() {
#ifndef NO_LOG
	Log("LOG_END <=======\n");

	fclose(LOG_FILE_HANDLE);

#endif
}

void *smalloc(size_t size) {
	void *ret = malloc(size);
	eassert(ret != NULL);
	memset(ret, 0, size);
	return ret;
}
