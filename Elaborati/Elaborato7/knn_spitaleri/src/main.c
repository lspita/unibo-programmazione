#include "../config.h"
#include "../inc/csv.h"
#include "../inc/loan.h"
#include "../inc/normalization.h"
#include "../inc/results.h"
#include "../inc/utils.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

int main()
{
    size_t rows = 0, cols = 0;
    csv_dimension(DATASET, CSV_HEADER, &rows, &cols);

    const size_t N_TRAINING = (size_t)(rows * (TRAINING_PERCENTAGE / 100.0));
    const size_t TRAINING_START_INDEX = rows - N_TRAINING;
    const LOAN AVERAGE_LOAN = get_average(DATASET, CSV_HEADER, rows, cols);
    putchar('\n');

    size_t correct_counter = 0;
    size_t total_counter = 0;

    size_t start = 0;
    puts("Check config.h for more options");
    do
    {
        printf("Select start of range [0 - %lu] (default: 0): ", TRAINING_START_INDEX - 1);
        scanf("%lu", &start);
        fflush_stdin();
    } while (start < 0 || start >= TRAINING_START_INDEX);

    size_t end = start;
    do
    {
        printf("Select end of range [%lu - %lu] (default: %lu): ", start, TRAINING_START_INDEX - 1, end);
        scanf("%lu", &end);
        fflush_stdin();
    } while (end < start || end >= TRAINING_START_INDEX);

    for (size_t i = start; i <= end; i++)
    {
        char current_datarow[cols][BUFFER_SIZE];
        csv_at(DATASET, CSV_HEADER, i, current_datarow);
        LOAN loan = loan_parse_data(current_datarow, &AVERAGE_LOAN);

        RESULT results[K];
        init_distance_results(results, K);

        for (size_t j = TRAINING_START_INDEX; j < rows; j++)
        {
            printf("Distance %lu - %lu: ", i, j);

            char match_datarow[cols][BUFFER_SIZE];
            csv_at(DATASET, CSV_HEADER, j, match_datarow);
            LOAN match = loan_parse_data(match_datarow, &AVERAGE_LOAN);

            double distance = loan_distance(&loan, &match);
            printf("%f\n", distance);

            RESULT res = {distance, match};
            confront_result(results, K, &res);
        }

        const int FILE_CATEGORY = loan.credit_policy;
        const int KNN_CATEGORY = get_category(results, K);

        total_counter++;
        if (FILE_CATEGORY == KNN_CATEGORY)
        {
            correct_counter++;
        }

        clear_console();
        printf("\nCurrent loan: ");
        print_loan(&loan, true);
        printf("\n\nK Nearest Neighbours:\n");
        print_results(results, K, true);
        printf("Category from file: %d\n", FILE_CATEGORY);
        printf("Category from KNN: %d\n", KNN_CATEGORY);

        pause();
    }

    printf("\nRun completed\n\n");
    print_correct(correct_counter, total_counter);
    return 0;
}