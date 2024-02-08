#include "../inc/hypotesis.h"
#include "../inc/input.h"
#include "../inc/utils.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#define CONSOLE_TMP_DATASET "console_dataset.csv"

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        fprintf(stderr, "Need at least 1 argument.\nOptions:\n<executable> console\n<executable> file <filename>\n");
    }
    const bool CONSOLE = strcmp(argv[1], "console") == 0;
    const char *const filename = CONSOLE ? CONSOLE_TMP_DATASET : argv[2];

    bool loop = true;
    if (CONSOLE)
    {
        create_file(filename);

        while (loop)
        {
            hypotesis h = input_hypotesis(true);

            serialize_hypotesis(&h, filename);

            char response = yesno_question("\nContinue?");
            loop = response == 'y';
        }
    }

    hypotesis general = calc_general_hypotesys(filename);

    loop = true;
    size_t counter = 0;
    while (loop)
    {
        printf("%sIteration #%lu\n\n", (counter > 0 ? "\n\n" : ""), counter);
        print_hypotesis("Most generic positive", &general);

        hypotesis h = input_hypotesis(false);

        h.wait = is_positive_hypotesis(&general, &h);
        printf("\nThe hypotesis results %s\n", (h.wait ? "positive" : "negative"));

        char response = yesno_question("Is it correct?");
        if (response == 'n')
        {
            puts("Inverting the value");
            h.wait = !h.wait;
        }

        response = yesno_question("Add the hypotesis to the dataset?");
        if (response == 'y')
        {
            serialize_hypotesis(&h, filename);
            general = calc_general_hypotesys(filename);
        }

        response = yesno_question("Continue?");
        loop = response == 'y';

        counter++;
    }

    if (CONSOLE)
    {
        char response = yesno_question("Save the dataset?");
        if (response == 'y')
        {
            char new_filename[1024] = {0};
            printf("Output file name: ");
            scanf("%s", new_filename);
            fflush_stdin();

            puts("Copying file contents...");
            copy_file(filename, new_filename);
        }

        puts("Deleting the console temporary dataset file");
        secure_fdelete(filename);
    }

    return 0;
}