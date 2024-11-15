#ifndef GLOBALS_H
#define GLOBALS_H

#include <assert.h>
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct System {
	uint8_t memory[4096];
	uint8_t registers[16];
	uint16_t addr_reg; // 12 bits

	uint16_t stack[16];
	int stack_ptr;

	uint16_t program_counter; // 12 bits;

	uint8_t display[8][4]; // d[x][y]
} System;

#ifndef LOG_LEVEL
#define LOG_LEVEL 1
#endif

char LOG_FILE[64];
FILE *LOG_FILE_HANDLE;

void log_init(char *log_file);

void log_close();

#define eassert(e)                                                             \
	if (!(e)) {                                                                \
		__eassert(#e);                                                         \
	}

#define __eassert(e)                                                           \
	Log(1, "Failed assertion '%s' with err \"%s\"\n", e, strerror(errno));     \
	exit(EXIT_FAILURE);

#define Log(level, pattern, ...)                                               \
	do {                                                                       \
		__Log(level, pattern, ##__VA_ARGS__);                                  \
	} while (0);

#ifndef NO_LOG
#define __Log(level, pattern, ...)                                             \
	do {                                                                       \
		if (LOG_LEVEL >= level) {                                              \
			fprintf(LOG_FILE_HANDLE, "[%s.%s:%d] " pattern, __FILE_NAME__,     \
					__FUNCTION__, __LINE__, ##__VA_ARGS__);                    \
			printf("[%s.%s:%d] " pattern, __FILE_NAME__, __FUNCTION__,         \
				   __LINE__, ##__VA_ARGS__);                                   \
			fflush(LOG_FILE_HANDLE);                                           \
		}                                                                      \
	} while (0);
#else
#define __Log(level, pattern, ...) noop();
#endif // NO_LOG

#define noop() ((void)0);

void *smalloc(size_t size);

#endif // GLOBALS_H