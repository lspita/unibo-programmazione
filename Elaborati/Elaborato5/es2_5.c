#include <stdbool.h>
#include <stdio.h>
#include <string.h>

void flush_stdin()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

unsigned long length(const char *str, const unsigned long max, const bool lib)
{
    unsigned long l = 0;
    if (lib)
    {
        l = strlen(str);
    }
    else
    {
        unsigned long count = 0;
        while (str[count] != '\0')
        {
            count++;
        }

        l = count;
    }

    return l < max ? l - 1 : l;
}

int ncompare(const char *str1, const char *str2, const size_t length, const bool lib)
{
    if (lib)
    {
        return strncmp(str1, str2, length);
    }
    else
    {
        int compareVal = 0;
        for (unsigned int i = 0; i < length && str1[i] != '\0' && str2[i] != '\0'; i++)
        {
            compareVal = str1[i] - str2[i];
            if (compareVal != 0)
            {
                break;
            }
        }
        return compareVal;
    }
}

#define INPUT_LEN 101

int main()
{
    char str1[INPUT_LEN];

    char res = '\0';
    do
    {
        printf("Use library <string.h>? [y/n]: ");
        res = getchar();
        flush_stdin();
    } while (res != 'y' && res != 'n');

    const bool USELIB = res == 'y';

    printf("Enter first input [max length: %d]: ", INPUT_LEN - 1);
    fgets(str1, INPUT_LEN, stdin);
    // flush_stdin();

    const unsigned long LEN1 = length(str1, INPUT_LEN, USELIB);

    char str2[LEN1 + 1];
    printf("Enter second input [max length: %lu]: ", LEN1);
    fgets(str2, LEN1 + 1, stdin);
    // flush_stdin();

    const unsigned long LEN2 = length(str2, LEN1 + 1, USELIB);

    char buffer[LEN1];

    unsigned long j = 0;
    for (unsigned long i = 0; i <= LEN1 - LEN2; i++)
    {
        int comparing = ncompare(&str1[i], str2, LEN2, USELIB);
        if (comparing == 0)
        {
            buffer[j] = '*';
            i += LEN2 - 1; // -1 because there is i++ in the for loop
        }
        else
        {
            buffer[j] = str1[i];
        }
        j++;
    }

    buffer[j] = '\0';
    printf("Result: %s\n", buffer);

    return 0;
}