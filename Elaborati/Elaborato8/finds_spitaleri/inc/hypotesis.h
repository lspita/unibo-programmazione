#ifndef HYPOTESIS_H
#define HYPOTESIS_H

#include "../inc/csv.h"
#include <stdbool.h>

#define N_HYPOTESIS_FIELDS 10 // number of fields in hypotesis

#define SPECIFIC_INT -1
#define SPECIFIC_CHAR '!'

#define GENERIC_INT -2
#define GENERIC_CHAR '?'

// crowded
#define EMPTY 0
#define PARTIAL 1
#define FULL 2

// price_level
#define CHEAP 0
#define NORMAL_PRICE 1
#define LUXURY 2

// type
#define ITALIAN 0
#define FRENCH 1
#define FAST_FOOD 2
#define THAI 3

// eta
#define LESS_10 0
#define _10_29 1
#define _30_60 2
#define MORE_60 3

typedef struct hypotesis
{
    int fields[N_HYPOTESIS_FIELDS];
    bool wait;
} hypotesis;

void fill_hypotesis(const char (*datarow)[CSV_COL_BUFFER_SIZE], hypotesis *result);
void hypotesis_compare(hypotesis *const general, const hypotesis *const match);
void print_hypotesis(const char *const name, const hypotesis *const h);
hypotesis calc_general_hypotesys(const char *const filename);
bool is_positive_hypotesis(hypotesis *const general, const hypotesis *const match);
hypotesis input_hypotesis(const bool input_positive);
void serialize_hypotesis(const hypotesis *const h, const char *const filename);

#endif