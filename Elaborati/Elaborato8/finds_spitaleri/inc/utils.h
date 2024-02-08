#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>
#include <stdio.h>

FILE *secure_fopen(const char *const filename, const char *const modes);
void create_file(const char *const filename);
void secure_fdelete(const char *const filename);
void copy_file(const char *const source_name, const char *const dest_name);
bool strbool(const char *const s);

#endif