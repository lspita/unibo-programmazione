#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "index.h"

#define FILENAME "datasetTmp.csv"

int main(int argc, char *argv[])
{
    int ok = checkArgument(argc, argv);
    if (ok == 1)
    {
        FILE *fptr = secure_fopen(FILENAME, "w+");
        if (fptr == NULL)
        {
            return 0;
        }
        InsertTitle(fptr);
        int l = CountFields(TakeRow(fptr, 0, 1));
        if (l < 2)
        {
            printf("ERROR. Not enough fields");
            return 0;
        }
        char **Title = findTitle(fptr);
        char q = 'z';
        do
        {
            q = 'z';
            char **New = InsertValue("\nEnter the value of the string to add to the temporary dataset\n\n", Title, l, -1);
            Add(New, l, fptr, 0);

            do
            {
                fflush_stdin();
                printf("Do you want to add a new Row? (y/n)");
                scanf("%c", &q);
            } while (q != 'y' && q != 'n');
        } while (q == 'y');

        int Field = inputInt("\nEnter the number of the Field To Compare: [0=First] ", 0, l - 1);
        fflush_stdin();
        printf("Enter the value of %s to search the pattern: ", TakeField(TakeRow(fptr, 0, 1), ",", Field));
        char *Value = getInput();
        char **Pattern = findPattern(fptr, Field, Value);

        ShowPattern(Title, Pattern, l);

        char **New = InsertValue("\nEnter the value of the string to confront\n\n", Title, l, Field);
        Compare(New, Pattern, l, Field, Title);

        secure_free(Title, l);
        secure_free(Pattern, l);
        secure_free(New, l);
        fclose(fptr);
    }
    else if (ok == 2)
    {
        FILE *fptr = secure_fopen(argv[2], "r+");
        if (fptr == NULL)
        {
            return 0;
        }

        int l = CountFields(TakeRow(fptr, 0, 1));
        int Field = inputInt("\nEnter the number of the Field To Compare: [0=First] ", 0, l - 1);
        fflush_stdin();
        printf("Enter the value of %s to search the pattern: ", TakeField(TakeRow(fptr, 0, 1), ",", Field));
        char *Value = getInput();
        char **Pattern = findPattern(fptr, Field, Value);
        char **Title = findTitle(fptr);

        ShowPattern(Title, Pattern, l);

        char **New = InsertValue("\nEnter the value of the string to confront\n\n", Title, l, Field);
        Compare(New, Pattern, l, Field, Title);

        fflush_stdin();
        printf("Enter the real value of '%s': ", TakeField(TakeRow(fptr, 0, 1), ",", Field));
        New[Field] = getInput();
        Add(New, l, fptr, 1);

        secure_free(Title, l);
        secure_free(Pattern, l);
        secure_free(New, l);
        fclose(fptr);
    }

    return 1;
}