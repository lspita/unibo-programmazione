#ifndef CSV_H
#define CSV_H

#include <stdbool.h>
#include <stddef.h>

#define CSV_COL_BUFFER_SIZE 512

typedef struct file_cache {
  size_t row;
  unsigned long long offset;
} file_cache;

void csv_dimension(const char *const filename, const bool header,
                   size_t *outrows, size_t *outcols);
void csv_at(const char *const filename, const bool header, const size_t row,
            char (*result)[CSV_COL_BUFFER_SIZE]);
void csv_add(const char *const filename, const size_t cols,
             char (*datarow)[CSV_COL_BUFFER_SIZE]);

#endif