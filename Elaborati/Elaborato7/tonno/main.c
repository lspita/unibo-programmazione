#include "utilities.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#define NLINE 9578
int main()
{
    FILE *fptr = secure_fopen("dataset.csv", "r");
    int K_value = inputInt("Enter the k-value:", 0, NLINE);
    int N_learning = NLINE;
    int N_SOnCalc = inputInt("\n0) All\n1) Specific index\n\nSelect an option [0 - 1]: ", 0, 1);
    int N_FOnCalc;
    if (N_SOnCalc == 0)
    {
        N_FOnCalc = 9578;
    }
    else
    {
        N_FOnCalc = inputInt("Enter the index:", 0, 9578);
        N_SOnCalc = N_FOnCalc;
    }

    for (int x = N_SOnCalc; x <= N_FOnCalc; x++)
    {
        char *tmpFieldToCompare = TakeRow(fptr, x);
        printf("%d) real loan:   %scalculated value:\n", x, tmpFieldToCompare);
        struct DataNode Values[N_learning];
        for (int y = 0; y < N_learning; y++)
        {
            Values[y].Distance = 0;
            char *tmpCompare = TakeRow(fptr, y);

            for (int z = 2; z <= 13; z++)
            {
                Values[y].Distance += pow(CharToDouble(TakeField(tmpFieldToCompare, ",", z)) -
                                              CharToDouble(TakeField(tmpCompare, ",", z)),
                                          2);
            }
            Values[y].Distance = sqrt(Values[y].Distance);

            Values[y].credit_policy = CharToDouble(TakeField(tmpCompare, ",", 0));
        }
        printBest(Values, N_learning, K_value);
    }
    fclose(fptr);
    return 0;
}