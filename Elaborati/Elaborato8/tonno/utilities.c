#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <conio.h>

void fflush_stdin()
{
    if (!feof(stdin))
    {
        int c;
        while ((c = getchar()) != '\n' && c != EOF)
            ;
    }
}

FILE *secure_fopen(const char *const filename, const char *const modes)
{
    FILE *stream = fopen(filename, modes);
    if (stream == NULL)
    {
        printf("Cannot open %s in reading mode", filename);
    }
    return stream;
}

void *secure_malloc(unsigned long size)
{
    void *ptr = malloc(size);
    if (ptr == NULL)
    {
        printf("Cannot allocate new memory");
    }
    return ptr;
}

char *TakeField(char *string, char *Divider, int Field)
{
    int i = 0;
    char cpy[strlen(string)];
    strcpy(cpy, string);
    char *token = strtok(cpy, Divider);

    while (token != NULL && i < Field)
    {
        token = strtok(NULL, Divider);
        i += 1;
    }

    size_t token_length = strlen(token);
    if (token_length > 0 && token[token_length - 1] == '\n')
    {
        token_length--;
    }

    char *out = (char *)malloc(token_length + 1);
    if (out == NULL)
    {
        return NULL;
    }

    strncpy(out, token, token_length);
    out[token_length] = '\0';
    return out;
}

int inputInt(char *prnt, int min, int max)
{
    int x = 0, tmp;
    bool ok = false;
    do
    {

        printf("%s ", prnt);
        if (x != 0)
        {
            fflush_stdin();
        }
        else
        {
            x = 1;
        }

        if (scanf("%d", &tmp) != 1 || tmp < min || tmp > max)
        {
            ok = false;
        }
        else
        {
            ok = true;
        }

    } while (!ok);
    return tmp;
}

char *getInput()
{
    int buffer_size = 10, position = 0, c;
    char *buffer = secure_malloc(buffer_size * sizeof(char));

    while (1)
    {
        c = getchar();
        if (c == EOF || c == '\n')
        {
            buffer[position] = '\0';
            break;
        }
        else
        {
            buffer[position] = c;

            if (position >= buffer_size - 1)
            {
                buffer_size += 10;
                buffer = (char *)realloc(buffer, buffer_size * sizeof(char));

                if (buffer == NULL)
                {
                    printf("Errore di riallocazione della memoria.\n");
                    exit(EXIT_FAILURE);
                }
            }

            position++;
        }
    }

    return buffer;
}

char *TakeRow(FILE *fptr, int Row, int Rewind)
{
    if (Rewind == 1)
    {
        rewind(fptr);
    }
    char str[100];
    int i = 0;

    while (fgets(str, 100, fptr) && i < Row)
    {
        i += 1;
    }

    size_t line_length = strlen(str);
    if (line_length > 0 && str[line_length - 1] == '\n')
    {
        line_length--;
    }

    char *out = (char *)malloc(line_length + 1);
    if (out == NULL)
    {
        return NULL;
    }

    strncpy(out, str, line_length);
    out[line_length] = '\0';
    return out;
}

int substring_count(char *string, char *substring)
{
    int i, l1, l2;
    int count = 0;
    l1 = strlen(string);
    l2 = strlen(substring);

    for (i = 0; i < l1 - l2 + 1; i++)
    {
        if (strstr(string + i, substring) == string + i)
        {
            count++;
            i = i + l2 - 1;
        }
    }
    return count;
}

int CountFields(char *string)
{
    return substring_count(string, ",") + 1;
}

int CountRow(FILE *fptr)
{
    rewind(fptr);
    char str[100];
    int i = 0;

    while (fgets(str, 100, fptr))
    {
        i += 1;
    }
    return i;
}

void secure_free(char **arr, int len)
{
    for (int i = 0; i < len; i++)
    {
        free(arr[i]);
    }
    free(arr);
}
