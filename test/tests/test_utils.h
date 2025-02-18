#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include "../../src/includes.h"
#include <stdint.h>

bool memassert(uint8_t *actual, uint8_t *expected, uint16_t loc, int size);

#define run_test(test, sys)                                                    \
	do {                                                                       \
		char *message = test(sys);                                             \
		tests_run++;                                                           \
		if (message != NULL)                                                   \
			return message;                                                    \
		tests_passed++;                                                        \
	} while (0)

#endif // TEST_UTILS_H
