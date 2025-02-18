#include "test_utils.h"
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

bool memassert(uint8_t *actual, uint8_t *expected, uint16_t loc, int n) {
	bool same = true;
	uint8_t act, exp;

	for (uint8_t i = 0; i < n; i++) {
		act = actual[loc + i];
		exp = expected[i];
		if (act != exp) {
			same = false;
			Log("memassert[%X]: exp: %X != act: %X\n", loc + i, act, exp);
		}
	}

	return same;
}
