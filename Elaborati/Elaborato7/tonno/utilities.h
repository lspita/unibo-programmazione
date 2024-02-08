#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

void fflush_stdin();
void *secure_malloc(unsigned long size);
FILE *secure_fopen(const char *const filename, const char *const modes);
int inputInt(char *prnt, int min, int max);
char *TakeField(char *string, char *Divider, int Field);
char *TakeRow(FILE *fptr, int Row);
double CharToDouble(char *x);

struct DataNode
{
    double Distance;
    int credit_policy;
};

struct DataNode *TakeNBest(struct DataNode *arr, int len, int k);
void printBest(struct DataNode *Values, int N_learning, int K_value);