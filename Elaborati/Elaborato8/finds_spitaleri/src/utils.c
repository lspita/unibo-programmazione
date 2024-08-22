#include "../inc/utils.h"
#include <stdio.h>

FILE *secure_fopen(const char *const filename, const char *const modes) {
  FILE *stream = fopen(filename, modes);
  if (stream == NULL) {
    fprintf(stderr, "Cannot open %s in %s mode", filename, modes);
  }
  return stream;
}

void create_file(const char *const filename) {
  FILE *stream = secure_fopen(filename, "w");
  fclose(stream);
}

void secure_fdelete(const char *const filename) {
  if (remove(filename) != 0) {
    fprintf(stderr, "Error deleting %s", filename);
  }
}

void copy_file(const char *const source_name, const char *const dest_name) {
  FILE *source = secure_fopen(source_name, "rb");
  FILE *dest = secure_fopen(dest_name, "wb");

  char c;
  while ((c = fgetc(source)) != EOF) {
    fputc(c, dest);
  }

  fclose(source);
  fclose(dest);
}

bool strbool(const char *const s) { return s[0] != '0'; }