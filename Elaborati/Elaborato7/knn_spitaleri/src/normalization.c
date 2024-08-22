#include "../inc/normalization.h"
#include "../inc/csv.h"
#include "../inc/utils.h"
#include <math.h>
#include <stdio.h>

LOAN get_average(const char *const filename, const bool header,
                 const size_t rows, const size_t cols) {
  // to ensure sums mantain decimals each step
  long double int_rate = 0.0;
  long double installment = 0.0;
  long double log_annual_inc = 0.0;
  long double dti = 0.0;
  long double fico = 0.0;
  long double days_with_cr_line = 0.0;
  long double revol_bal = 0.0;
  long double revol_util = 0.0;
  long double inq_last_6mths = 0.0;
  long double delinq_2yrs = 0.0;
  long double pub_rec = 0.0;
  long double not_fully_paid = 0.0;

  for (size_t i = 0; i < rows; i++) {
    clear_console();
    printf("Calculating average: (%lu/%lu)\n", i + 1, rows);

    char datarow[cols][BUFFER_SIZE];
    csv_at(DATASET, CSV_HEADER, i, datarow);
    LOAN loan = loan_parse_data(datarow, NULL);

    // skip credit_policy
    // skip purpose
    int_rate += loan.int_rate / rows;
    installment += loan.installment / rows;
    log_annual_inc += loan.log_annual_inc / rows;
    dti += loan.dti / rows;
    fico += (double)loan.fico / rows;
    days_with_cr_line += loan.days_with_cr_line / rows;
    revol_bal += (double)loan.revol_bal / rows;
    revol_util += loan.revol_util / rows;
    inq_last_6mths += (double)loan.inq_last_6mths / rows;
    delinq_2yrs += (double)loan.delinq_2yrs / rows;
    pub_rec += (double)loan.pub_rec / rows;
    not_fully_paid += (double)loan.not_fully_paid / rows;
  }

  LOAN average = {
      -1,
      "",
      int_rate,
      installment,
      log_annual_inc,
      dti,
      round(fico),
      days_with_cr_line,
      round(revol_bal),
      revol_util,
      round(inq_last_6mths),
      round(delinq_2yrs),
      round(pub_rec),
      round(not_fully_paid) > 0 ? true : false,
  };

  clear_console();
  printf("Average loan: ");
  print_loan(&average, false);
  return average;
}