#ifndef NORMALIZATION_H
#define NORMALIZATION_H

#include "loan.h"
#include <stddef.h>

LOAN get_average(const char *const filename, const bool header, const size_t rows, const size_t cols);

#endif