#include "index.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_VALUE 50
#define COLUMN_WIDTH 14
int checkArgument(int argc, char *argv[]) {
  if (argc < 2) {
    printf("\n\nERROR: no argument passed through command line!");
  } else if (argc > 3) {
    printf("\n\nERROR: too much argument passed through command line!");
  } else {
    if (0 == strcmp(argv[1], "file")) {
      if (argc < 3) {
        printf("\n\nERROR: enter the file name");
      } else {
        return 2;
      }
    } else if (0 == strcmp(argv[1], "manual")) {
      if (argc > 2) {
        printf("\n\nERROR: too much argument passed through command line!");
      } else {
        return 1;
      }
    } else {
      printf("\n\nERROR: invalid command");
    }
  }
  return 0;
}

void printLine(int columnCount) {
  printf("\n");
  for (int i = 0; i < columnCount; i++) {
    printf("+");
    for (int j = 0; j < COLUMN_WIDTH + 2; j++) {
      printf("-");
    }
  }
  printf("+");
}

void printfTable(char *c[], int len) {
  printf("\n");
  for (int i = 0; i < len; i++) {
    printf("| %-*s ", COLUMN_WIDTH, c[i]);
  }
  printLine(len);
}

void FullFill(char **result, int len) {
  for (int i = 0; i < len; i++) {
    result[i] = (char *)malloc((MAX_VALUE) * sizeof(char));
    strcpy(result[i], "!");
  }
}

char **findPattern(FILE *fptr, int NField, char *Value) {
  int lenRow = CountRow(fptr);
  char *Row = TakeRow(fptr, 0, 1);
  int lenField = CountFields(Row);

  char **result = (char **)malloc(lenField * sizeof(char *));
  if (result == NULL) {
    printf("Errore di allocazione della memoria per result.\n");
    exit(EXIT_FAILURE);
  }

  FullFill(result, lenField);

  for (int i = 1; i < lenRow; i++) {
    Row = TakeRow(fptr, i, 1);
    if (strcmp(TakeField(Row, ",", NField), Value) == 0) {
      for (int j = 0; j < lenField; j++) {
        char *tmp = TakeField(Row, ",", j);
        if (strcmp(result[j], "!") == 0) {
          result[j] = tmp;
        } else if (strcmp(result[j], tmp) != 0) {
          result[j] = "?";
        }
      }
    }
  }

  return result;
}

char **findTitle(FILE *fptr) {
  char *Row = TakeRow(fptr, 0, 1);
  int lenField = CountFields(Row);
  char **result = (char **)malloc(lenField * sizeof(char *));
  for (int i = 0; i < lenField; i++) {
    result[i] = (char *)malloc((MAX_VALUE) * sizeof(char));
    strcpy(result[i], TakeField(Row, ",", i));
  }
  return result;
}

char **InsertValue(char *prnt, char **Title, int l, int Field) {
  printf("%s", prnt);
  char **result = (char **)malloc(l * sizeof(char *));
  for (int i = 0; i < l; i++) {
    result[i] = secure_malloc((MAX_VALUE) * sizeof(char));
    if (i != Field) {
      printf("%s-> ", Title[i]);

      scanf("%s", result[i]);
      printf("\n");
    }
  }
  return result;
}

void InsertTitle(FILE *fptr) {
  char tmp[500];
  printf("Enter all titles separated by a comma:\n");
  gets(tmp);
  fprintf(fptr, "%s", tmp);
}

int Compare(char **StringToCompare, char **Comparator, int l, int NField,
            char **Title) {
  int ok = 1;
  for (int i = 0; i < l; i++) {
    if (strcmp(Comparator[i], "?") != 0) {
      if (strcmp(Comparator[i], StringToCompare[i]) != 0 &&
          strcmp(Comparator[i], "!") != 0 && i != NField) {
        if (ok == 1) {
          printf("\nThe case you entered does not respect the general case for "
                 "attribute-->");
          ok = 0;
        }
        printf("  %s  ", Title[i]);
      }
    }
  }
  if (ok == 1)
    printf("\nThe case you entered respect the general case to have as "
           "attribute '%s'-->%s \n",
           Title[NField], Comparator[NField]);
  else
    printf("\n");
  return ok;
}

int Add(char **ToAdd, int l, FILE *fptr, int ask) {
  char q;
  if (ask != 0) {
    do {
      fflush_stdin();
      printf("Do you want to add the Row to the dataSet? (y/n)");
      scanf("%c", &q);
    } while (q != 'y' && q != 'n');
  }

  if (l != CountFields(TakeRow(fptr, 0, 1)) || q == 'n') {
    if (q != 'n')
      printf("ERROR");
    return 0;
  }

  fseek(fptr, 0, SEEK_END);
  fprintf(fptr, "\n");
  for (int i = 0; i < l; i++) {
    if (i == l - 1)
      fprintf(fptr, "%s", ToAdd[i]);
    else
      fprintf(fptr, "%s,", ToAdd[i]);
  }
  if (ask != 0)
    printf("Row Added");
  return 1;
}

void ShowPattern(char **Title, char **Pattern, int l) {
  printf("THIS IS THE PATTERN I FOUND:");
  printLine(l);
  printfTable(Title, l);
  printfTable(Pattern, l);
}