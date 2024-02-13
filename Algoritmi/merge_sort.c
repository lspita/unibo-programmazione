#include <stdbool.h>
#include <stdio.h>

#define SIZE 11

void merge_sort(const int *const array, const int start, const int size, int *out_sorted)
{
    if (size == 0)
        return;

    if (size == 1)
    {
        out_sorted[0] = array[start];
        return;
    }

    const int half_size = (int)(size / 2);
    // sort halves
    int starts[2] = {start, start + half_size};
    int sizes[2] = {half_size, size - half_size};
    int halves[2][sizes[1]]; // extra cell if odd

    merge_sort(array, starts[0], sizes[0], halves[0]); // from 0 to half
    merge_sort(array, starts[1], sizes[1], halves[1]); // from half to last

    // merge halves
    int i_sorted = 0;
    bool finished[2] = {false, false};
    int indexes[2] = {0, 0};

    while (!finished[0] || !finished[1])
    {
        for (int current_half = 0; current_half < 2; current_half++)
        {
            int other_half = current_half == 0 ? 1 : 0;

            for (int j = indexes[current_half]; j < sizes[current_half]; j++)
            {
                if (finished[other_half] ||
                    (!finished[current_half] &&
                     halves[current_half][j] < halves[other_half][indexes[other_half]]))
                {
                    out_sorted[i_sorted] = halves[current_half][j];
                    i_sorted++;
                    indexes[current_half]++;
                }
                else
                    break;
            }
        }

        finished[0] = indexes[0] == sizes[0];
        finished[1] = indexes[1] == sizes[1];
    }
}

void print_array(int *array, int size)
{
    for (int i = 0; i < size; i++)
    {
        printf("%d ", array[i]);
    }
    putchar('\n');
}

int main()
{
    int array[SIZE] = {5, 3, 4, 6, 7, 9, 0, 1, 8, 2, 10};
    int sorted[SIZE] = {0};
    print_array(array, SIZE);
    merge_sort(array, 0, SIZE, sorted);
    print_array(sorted, SIZE);
    return 0;
}