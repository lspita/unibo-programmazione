#include "../inc/hypotesis.h"
#include "../inc/csv.h"
#include "../inc/input.h"
#include "../inc/utils.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// datarow == NULL => specific hypotesis
void fill_hypotesis(const char (*datarow)[CSV_COL_BUFFER_SIZE], hypotesis *result)
{
    result->wait = datarow == NULL;
    for (size_t i = 0; i < N_HYPOTESIS_FIELDS; i++)
    {
        result->fields[i] = (datarow == NULL ? SPECIFIC_INT : atoi(datarow[i]));
    }
    if (datarow != NULL)
    {
        result->wait = strbool(datarow[N_HYPOTESIS_FIELDS]);
    }
}

void hypotesis_compare(hypotesis *const general, const hypotesis *const match)
{
    if (!match->wait)
    {
        return;
    }

    for (size_t i = 0; i < N_HYPOTESIS_FIELDS; i++)
    {
        if (general->fields[i] == SPECIFIC_INT)
        {
            general->fields[i] = match->fields[i];
        }
        else if (general->fields[i] != match->fields[i])
        {
            general->fields[i] = GENERIC_INT;
        }
    }
}

void print_hypotesis(const char *const name, const hypotesis *const h)
{
    printf("%s: ", name);
    putchar('[');
    for (size_t i = 0; i < N_HYPOTESIS_FIELDS; i++)
    {
        int field = h->fields[i];
        switch (field)
        {
            case SPECIFIC_INT:
                putchar(SPECIFIC_CHAR);
                break;
            case GENERIC_INT:
                putchar(GENERIC_CHAR);
                break;
            default:
                printf("%d", field);
                break;
        }
        printf(", ");
    }
    printf("\b\b] result: %d\n", h->wait);
}

hypotesis calc_general_hypotesys(const char *const filename)
{
    size_t rows = 0, cols = 0;
    csv_dimension(filename, false, &rows, &cols);

    hypotesis general = {};
    fill_hypotesis(NULL, &general);

    for (size_t i = 0; i < rows; i++)
    {
        char datarow[N_HYPOTESIS_FIELDS + 1][CSV_COL_BUFFER_SIZE];
        csv_at(filename, false, i, datarow);

        hypotesis h = {};
        fill_hypotesis(datarow, &h);
        hypotesis_compare(&general, &h);
    }

    return general;
}

bool is_positive_hypotesis(hypotesis *const general, const hypotesis *const match)
{
    for (size_t i = 0; i < N_HYPOTESIS_FIELDS; i++)
    {
        if (general->fields[i] != GENERIC_INT && general->fields[i] != match->fields[i])
        {
            return false;
        }
    }

    return true;
}

hypotesis input_hypotesis(const bool input_positive)
{
    printf("Insert new hypotesis (general: %d, specific: %d)\n", GENERIC_INT, SPECIFIC_INT);

    hypotesis h = {};

    h.fields[0] = limited_input("alternative", GENERIC_INT, 1);
    h.fields[1] = limited_input("bar", GENERIC_INT, 1);
    h.fields[2] = limited_input("weekday", GENERIC_INT, 1);
    h.fields[3] = limited_input("hungry", GENERIC_INT, 1);
    h.fields[4] = limited_input("crowded", GENERIC_INT, FULL);
    h.fields[5] = limited_input("price_level", GENERIC_INT, LUXURY);
    h.fields[6] = limited_input("raining", GENERIC_INT, 1);
    h.fields[7] = limited_input("booked", GENERIC_INT, 1);
    h.fields[8] = limited_input("type", GENERIC_INT, THAI);
    h.fields[9] = limited_input("eta", GENERIC_INT, MORE_60);

    if (input_positive)
    {
        h.wait = limited_input("wait", 0, 1);
    }

    return h;
}

void serialize_hypotesis(const hypotesis *const h, const char *const filename)
{
    const size_t cols = N_HYPOTESIS_FIELDS + 1;
    char datarow[cols][CSV_COL_BUFFER_SIZE];
    for (size_t i = 0; i < cols - 1; i++)
    {
        sprintf(datarow[i], "%d", h->fields[i]);
    }
    sprintf(datarow[cols - 1], "%d", h->wait);

    csv_add(filename, cols, datarow);
}