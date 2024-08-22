#include "../inc/utils.h"
#include "../config.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

void fflush_stdin() {
  if (!feof(stdin)) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
      ;
  }
}

FILE *secure_fopen(const char *const filename, const char *const modes) {
  FILE *stream = fopen(filename, modes);
  if (stream == NULL) {
    fprintf(stderr, "Cannot open %s in %s mode", filename, modes);
  }
  return stream;
}

void clear_console() { printf("\e[1;1H\e[2J"); }

void pause() {
  if (PAUSE) {
    printf("\nPress ENTER to continue...");
    getchar();
  }
}

float percentage(const size_t n, const size_t total) {
  return (float)n / total * 100;
}

void print_correct(const size_t correct_counter, const size_t total_counter) {
  printf("Correct: %lu/%lu (%.3f%%)\n", correct_counter, total_counter,
         percentage(correct_counter, total_counter));
}