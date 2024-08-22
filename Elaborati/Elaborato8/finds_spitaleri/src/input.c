#include <stdio.h>

void fflush_stdin() {
  if (!feof(stdin)) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
      ;
  }
}

int limited_input(const char *const message, const int min, const int max) {
  int res = min - 1;
  do {
    printf("%s [min: %d, max: %d]: ", message, min, max);
    scanf("%d", &res);
    fflush_stdin();
  } while (res < min || res > max);
  return res;
}

char yesno_question(const char *const question) {
  char response = '\0';
  do {
    printf("%s [y/n]: ", question);
    response = getchar();
    fflush_stdin();
  } while (response != 'y' && response != 'n');

  return response;
}