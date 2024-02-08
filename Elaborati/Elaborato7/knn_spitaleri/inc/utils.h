#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>

void fflush_stdin();
FILE *secure_fopen(const char *const filename, const char *const modes);
void clear_console();
void pause();
float percentage(const size_t n, const size_t total);
void print_correct(const size_t correct_counter, const size_t total_counter);

#endif