#include "load_test.h"
#include "operations_test.h"
#include "test_utils.h"
#include <stdio.h>

int main(int argc, char **argv) {
	log_init();

	int tests_passed = 0;

	System sys;

	tests_passed += load_test(&sys);

	tests_passed += operations_test(&sys);

	return EXIT_SUCCESS;
}
