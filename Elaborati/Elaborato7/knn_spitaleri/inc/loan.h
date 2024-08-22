#ifndef LOAN_H
#define LOAN_H

#include "../config.h"
#include <stdbool.h>

typedef struct loan {
  int credit_policy;
  char purpose[BUFFER_SIZE];
  double int_rate;
  double installment;
  double log_annual_inc;
  double dti;
  int fico;
  double days_with_cr_line;
  int revol_bal;
  double revol_util;
  int inq_last_6mths;
  int delinq_2yrs;
  int pub_rec;
  bool not_fully_paid;
} LOAN;

LOAN empty_loan();
LOAN loan_parse_data(const char (*const datarow)[BUFFER_SIZE],
                     const LOAN *const average);
void print_loan(const LOAN *const loan, const bool normalized);
double loan_distance(const LOAN *const loan, const LOAN *const match);

#endif