#include "../inc/results.h"
#include <float.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

double loan_distance(const LOAN *const loan, const LOAN *const match) {
  double result = 0;

  result += strcmp(loan->purpose, loan->purpose) !=
            0; // 0 (false) distance if strings are equal, else 1 (true)
  result += fabs(loan->int_rate - match->int_rate);
  result += fabs(loan->installment - match->installment);
  result += fabs(loan->log_annual_inc - match->log_annual_inc);
  result += fabs(loan->dti - match->dti);
  result += abs(loan->fico - match->fico);
  result += fabs(loan->days_with_cr_line - match->days_with_cr_line);
  result += abs(loan->revol_bal - match->revol_bal);
  result += fabs(loan->revol_util - match->revol_util);
  result += abs(loan->inq_last_6mths - match->inq_last_6mths);
  result += abs(loan->delinq_2yrs - match->delinq_2yrs);
  result += abs(loan->pub_rec - match->pub_rec);
  result += abs(loan->not_fully_paid - match->not_fully_paid);

  return result;
}

void init_distance_results(RESULT *results, const size_t k) {
  for (RESULT *cell = &results[0]; cell <= &results[k - 1]; cell++) {
    *cell = (RESULT){.distance = DBL_MAX};
  }
}

void confront_result(RESULT *results, const size_t k, const RESULT *const res) {
  double max = -DBL_MAX;
  size_t max_index = 0;
  for (size_t i = 0; i < k; i++) {
    if (results[i].distance > max) {
      max = results[i].distance;
      max_index = i;
    }
  }

  if (res->distance < results[max_index].distance) {
    results[max_index] = *res;
  }
}

int get_category(RESULT *results, const size_t k) {
  size_t instances[N_CATEGORIES] = {0};

  for (RESULT *res = &results[0]; res <= &results[k - 1]; res++) {
    instances[res->loan.credit_policy]++;
  }

  int most_appearing_category = 0;
  for (size_t i = 0; i < N_CATEGORIES; i++) {
    if (instances[i] > instances[most_appearing_category]) {
      most_appearing_category = i;
    }
  }

  return most_appearing_category;
}

void print_results(const RESULT *const results, const size_t k,
                   const bool normalized) {
  for (const RESULT *res = &results[0]; res <= &results[k - 1]; res++) {
    printf("distance: %f\ncategory: %d\nloan: ", res->distance,
           res->loan.credit_policy);
    print_loan(&(res->loan), normalized);
    puts("\n");
  }
}