#ifndef OPERATIONS_H
#define OPERATIONS_H

#include "globals.h"
#include <stdint.h>

void execute(System *sys, uint16_t operation);

typedef void (*instruction)(System *, uint16_t);

#endif // OPERATIONS_H
