#include "../inc/csv.h"
#include "../inc/utils.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

void csv_dimension(const char *const filename, const bool header, size_t *outrows, size_t *outcols)
{
    size_t ncols = 0;
    size_t nrows = 0;

    FILE *stream = secure_fopen(filename, "r");

    char c = '\0';
    unsigned int linechars = 0;
    while ((c = fgetc(stream)) != '\n' && c != EOF)
    {
        if (c == ',')
        {
            ncols++;
        }
        else
        {
            linechars++;
        }
    }

    if (linechars > 0) // at least one element found
    {
        nrows++;
        if (ncols == 0)
        {
            ncols++;
        }

        linechars = 0;
        while ((c = fgetc(stream)) != EOF)
        {
            if (c == '\n')
            {
                nrows++;
                linechars = 0;
            }
            else
            {
                linechars++;
            }
        }
        if (linechars > 0) // last line not empty
        {
            nrows++;
        }
    }
    fclose(stream);
    *outrows = nrows - (header ? 1 : 0);
    *outcols = ncols + 1; // no trailing comma
}

int cached_fgetc(FILE *stream, FILE_CACHE *const cache)
{
    cache->offset++;
    return fgetc(stream);
}

void csv_at(const char *const filename, const bool header, const size_t row, char (*result)[BUFFER_SIZE])
{
    static FILE_CACHE cache = {-1, 0};
    char c = '\0';
    FILE *stream = secure_fopen(filename, "r");

    if (cache.row == -1 || row < cache.row)
    {
        cache = (FILE_CACHE){0, 0};
        if (header)
        {
            while ((c = cached_fgetc(stream, &cache)) != '\n' || c == EOF)
                ;
        }
    }

    fseek(stream, cache.offset, SEEK_SET);

    bool found = false;
    while (!found && (c = cached_fgetc(stream, &cache)) != EOF)
    {
        found = cache.row == row;
        if (found)
        {
            size_t i = 0;
            size_t j = 0;
            while (c != '\n' && c != EOF)
            {
                if (c != ',')
                {
                    result[i][j] = c;
                    j++;
                }
                else
                {
                    result[i][j] = '\0';
                    i++;
                    j = 0;
                }
                c = cached_fgetc(stream, &cache);
            }
            if (j > 0)
            {
                result[i][j] = '\0';
            }
            cache.row++;
        }
        else if (c == '\n')
        {
            cache.row++;
        }
    }
    fclose(stream);
}