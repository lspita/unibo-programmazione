#ifndef CSV_H
#define CSV_H

#include "../config.h"
#include <stdbool.h>
#include <stddef.h>

typedef struct file_cache {
  size_t row;
  unsigned long long offset;
} FILE_CACHE;

void csv_dimension(const char *const filename, const bool header,
                   size_t *outrows, size_t *outcols);
void csv_at(const char *const filename, const bool header, const size_t row,
            char (*result)[BUFFER_SIZE]);

#endif