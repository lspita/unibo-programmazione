#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

struct DataNode
{
    double Distance;
    bool credit_policy; // 0 <--
};

void fflush_stdin()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

FILE *secure_fopen(const char *const filename, const char *const modes)
{
    FILE *stream = fopen(filename, modes);
    if (stream == NULL)
    {
        fprintf(stderr, "Cannot open %s in reading mode", filename);
    }
    return stream;
}

void *secure_malloc(unsigned long size)
{
    void *ptr = malloc(size);
    if (ptr == NULL)
    {
        fprintf(stderr, "Cannot allocate new memory");
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
    char *out = secure_malloc((strlen(token) + 1) * sizeof(char));
    strcpy(out, token);
    return out;
}

char *TakeRow(FILE *fptr, int Row)
{
    rewind(fptr);
    char str[100];
    int i = 0;

    while (fgets(str, 100, fptr) && i < Row)
    {
        i += 1;
    }
    char *out = secure_malloc((strlen(str) + 1) * sizeof(char));
    strcpy(out, str);
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

double CharToDouble(char *char_array)
{
    // Rimuovi spazi bianchi o caratteri di nuova riga alla fine della stringa
    size_t length = strlen(char_array);
    while (length > 0 && (isspace(char_array[length - 1]) || char_array[length - 1] == '\n'))
    {
        char_array[--length] = '\0';
    }

    char *endptr;
    double result = strtod(char_array, &endptr);

    if (*endptr != '\0')
    {
        printf("Conversion of %s failed. Non-convertible character: %c", char_array, *endptr);
        return 1; // Ritorna un valore che indica un errore
    }
    return result;
}

void swap(struct DataNode *a, struct DataNode *b)
{
    struct DataNode temp = *a;
    *a = *b;
    *b = temp;
}

int partition(struct DataNode *arr, int low, int high)
{
    double pivot = arr[high].Distance;
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++)
    {
        if (arr[j].Distance < pivot)
        { // Modifica la condizione da > a <
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

void quickSort(struct DataNode *arr, int low, int high)
{
    if (low < high)
    {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

struct DataNode *TakeNBest(struct DataNode *arr, int len, int k)
{
    quickSort(arr, 0, len - 1);

    struct DataNode *out = (struct DataNode *)malloc(sizeof(struct DataNode) * k);
    for (int i = 0; i < k; i++)
    {
        out[i] = arr[i];
    }
    return out;
}

void printBest(struct DataNode *Values, int N_learning, int K_value)
{
    struct DataNode *bestNodes = TakeNBest(Values, N_learning, K_value);
    double credit_policy_sum = 0;
    for (int i = 0; i < K_value; i++)
    {
        credit_policy_sum += bestNodes[i].credit_policy;
    }
    printf("\n");
    credit_policy_sum = credit_policy_sum / K_value * 100;

    if (credit_policy_sum >= 50)
        printf("credit_policy: 1, probability %f%%\n", credit_policy_sum);
    else
        printf("credit_policy: 0, probability %f%%\n", 100 - credit_policy_sum);

    printf("-----------------------------------------------------------------------------------\n");
}
