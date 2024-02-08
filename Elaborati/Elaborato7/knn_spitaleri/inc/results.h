#ifndef RESULTS_H
#define RESULTS_H

#include "loan.h"
#include <stddef.h>

typedef struct result
{
    double distance;
    LOAN loan;
} RESULT;

double loan_distance(const LOAN *const loan, const LOAN *const match);
void init_distance_results(RESULT *results, const size_t k);
void confront_result(RESULT *results, const size_t k, const RESULT *const result);
int get_category(RESULT *results, const size_t k);
void print_results(const RESULT *const results, const size_t k, const bool normalized);

#endif