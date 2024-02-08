#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

void blue()
{
    printf("\033[1;34m");
}

void reset()
{
    printf("\033[0m");
}

int randint(int min, int max)
{
    return min + rand() % (max + 1 - min);
}

void flush_stdin()
{
    while (getchar() != '\n')
    {
    }
}

void clearConsole()
{
    printf("\e[1;1H\e[2J");
}

char *Encrypt(int *memory, char *array, int length)
{
    srand(time(NULL));
    char *newArray = (char *)malloc(length * sizeof(char));
    int i;
    for (i = 0; i < length; i++)
    {
        memory[i] = randint(1, CHAR_MAX - array[i] - 1); // char 127 = DEL
        newArray[i] = array[i] + memory[i];
    }
    newArray[i + 1] = '\0';
    return newArray;
}

void Decrypt(int *memory, char *array, int length)
{
    int x;
    for (int i = 0; i < length; i++)
    {
        if (memory[i] > 0)
        {
            x = randint(1, (memory[i] < 3 ? memory[i] : 3));
            array[i] = array[i] - x;
            memory[i] -= x;
        }
    }
}

bool printString(char *str, int length, char *original)
{
    clearConsole();
    bool result = true;
    for (int i = 0; i < length; i++)
    {
        if (str[i] == original[i])
        {
            blue();
        }
        else
        {
            result = false;
            reset();
        }
        printf("%c", str[i]);
    }
    return result;
}

int str_length(char str[])
{
    int count;
    for (count = 0; str[count] != '\0'; ++count)
        ;
    return count;
}

void startTimer(const double seconds)
{
    double elapsed = 0;
    const clock_t start = clock();
    do
    {
        elapsed = (clock() - start) / (double)CLOCKS_PER_SEC;
    } while (elapsed < seconds);
}

bool isPrintable(char *str, int len)
{
    bool ok;
    for (int i = 0; i < len - 1; i++)
    {
        ok = isprint(str[i]);
        if (ok == false)
        {
            return false;
        }
    }
    return true;
}

#define MAXLEN 500

int main()
{
    char inString[MAXLEN];
    bool ok = true;
    int length;
    do
    {
        clearConsole();
        printf("Enter a string [max 500 chars | only printable char]: ");
        fgets(inString, MAXLEN, stdin);
        length = str_length(inString) - 1;
        if (length + 1 > MAXLEN)
        {
            flush_stdin();
        }
        ok = isPrintable(inString, length);
    } while (ok == false);

    int memory[length];
    char *stringE = Encrypt(memory, inString, length);
    srand(time(NULL));
    ok = printString(stringE, length, inString);
    printf("\nPress enter to decrypt...");
    getchar();

    while (!ok)
    {
        Decrypt(memory, stringE, length);
        ok = printString(stringE, length, inString);
        startTimer(0.03);
    }

    reset();
    free(stringE);
}
