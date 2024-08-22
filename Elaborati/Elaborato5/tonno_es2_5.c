#include <stdbool.h>
#include <stdio.h>
#include <string.h>

bool check(char *s1, int len1, char *s2, int len2, int i, int lib) {
  bool ok;
  if (lib == 0) {
    ok = true;
    for (int j = 1; j < len2; j++) {
      if (s1[i + j] != s2[j]) {
        return false;
      }
    }
    return ok;
  } else {
    char tmp[len2];
    for (int j = i; j - i < len2; j++) {
      tmp[j - i] = s1[j];
    }
    int x = strncmp(tmp, s2, len2);
    return x == 0 ? true : false;
  }
}

int str_length(char str[]) {
  int count;
  for (count = 0; str[count] != '\0'; ++count)
    ;
  return count;
}

void flush_stdin() {
  while (getchar() != '\n') {
  }
}

void clearConsole() { printf("\e[1;1H\e[2J"); }

#define LENG 500
int main() {
  char s1[LENG];
  printf("Inserire stringa [max %d]:", LENG);
  fgets(s1, LENG, stdin);
  int len1 = str_length(s1) - 1;
  char s2[len1];
  printf("Inserire stringa [max %d]:", len1);
  fgets(s2, len1, stdin);
  int len2 = str_length(s2) - 1;
  bool ok;
  int outCount = 0;
  char result[len1];

  for (int i = 0; i < len1; i++) {
    if (s1[i] == s2[0]) {
      ok = check(s1, len1, s2, len2, i, 1);
      if (ok == true) {
        result[outCount++] = '*';
        i += len2 - 1;
      } else {
        result[outCount++] = s1[i];
      }
    } else {
      result[outCount++] = s1[i];
    }
  }
  for (int i = 0; i < outCount; i++) {
    printf("%c", result[i]);
  }
}