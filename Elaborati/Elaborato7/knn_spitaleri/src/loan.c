#include "../inc/loan.h"
#include "../inc/utils.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

LOAN empty_loan()
{
    return (LOAN){
        0,
        "",
        0.0,
        0.0,
        0.0,
        0.0,
        0,
        0.0,
        0,
        0,
        0,
        0,
        0,
        0,
    };
}

LOAN loan_parse_data(const char (*const datarow)[BUFFER_SIZE], const LOAN *const average)
{
    LOAN loan = {};
    LOAN avg = average == NULL ? empty_loan() : *average;

    loan.credit_policy = atoi(datarow[0]); // skip credit_policy normalization
    strcpy(loan.purpose, datarow[1]);      // skip purpose normalization
    loan.int_rate = strtod(datarow[2], NULL) - avg.int_rate;
    loan.installment = strtod(datarow[3], NULL) - avg.installment;
    loan.log_annual_inc = strtod(datarow[4], NULL) - avg.log_annual_inc;
    loan.dti = strtod(datarow[5], NULL) - avg.dti;
    loan.fico = atoi(datarow[6]) - avg.fico;
    loan.days_with_cr_line = strtod(datarow[7], NULL) - avg.days_with_cr_line;
    loan.revol_bal = atoi(datarow[8]) - avg.revol_bal;
    loan.revol_util = strtod(datarow[9], NULL) - avg.revol_util;
    loan.inq_last_6mths = atoi(datarow[10]) - avg.inq_last_6mths;
    loan.delinq_2yrs = atoi(datarow[11]) - avg.delinq_2yrs;
    loan.pub_rec = atoi(datarow[12]) - avg.pub_rec;
    loan.not_fully_paid = atoi(datarow[13]) == 1 - avg.not_fully_paid;

    return loan;
}

void print_loan(const LOAN *const loan, const bool normalized)
{
    printf("%s{"
           "\n\tcredit_policy: %d,"
           "\n\tpurpose: %s,"
           "\n\tint_rate: %f,"
           "\n\tinstallment: %f,"
           "\n\tlog_annual_inc: %f,"
           "\n\tdti: %f,"
           "\n\tfico: %d,"
           "\n\tdays_with_cr_line: %f,"
           "\n\trevol_bal: %d,"
           "\n\trevol_util: %f,"
           "\n\tinq_last_6mths: %d,"
           "\n\tdelinq_2yrs: %d,"
           "\n\tpub_rec: %d,"
           "\n\tnot_fully_paid: %d,"
           "\n}\n",
           normalized ? "(normalized) " : "",
           loan->credit_policy,
           loan->purpose,
           loan->int_rate,
           loan->installment,
           loan->log_annual_inc,
           loan->dti,
           loan->fico,
           loan->days_with_cr_line,
           loan->revol_bal,
           loan->revol_util,
           loan->inq_last_6mths,
           loan->delinq_2yrs,
           loan->pub_rec,
           loan->not_fully_paid);
}