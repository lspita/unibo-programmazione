#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int checkArgument(int argc, char *argv[]);
void fflush_stdin();
void *secure_malloc(unsigned long size);
FILE *secure_fopen(const char *const filename, const char *const modes);
int inputInt(char *prnt, int min, int max);
char *TakeField(char *string, char *Divider, int Field);
char *TakeRow(FILE *fptr, int Row, int Rewind);
int checkArgument(int argc, char *argv[]);
int CountRow(FILE *fptr);
int CountFields(char *string);
int substring_count(char *string, char *substring);
char **findPattern(FILE *fptr, int NField, char *Value);
int inputInt(char *prnt, int min, int max);
char *getInput();
void printfTable(char **c, int len);
int substring_count(char *string, char *substring);
char **findTitle(FILE *fptr);
void printLine(int columnCount);
void secure_free(char **arr, int len);
char **InsertValue(char *prnt, char **Title, int l, int Field);
int Compare(char **StringToCompare, char **Comparator, int l, int NField,
            char **Title);
int Add(char **ToAdd, int l, FILE *fptr, int ask);
void InsertTitle(FILE *fptr);
void ShowPattern(char **Title, char **Pattern, int l);
