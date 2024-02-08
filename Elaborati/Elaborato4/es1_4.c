// Abbiamo corretto la stampa dei byte ma riguardo l'intersezione con duplicati non abbiamo trovato errori.
// Abbiamo quindi chiesto conferma alla prof.ssa Carbonaro e anche a lei risulta corretto.

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void clearConsole()
{
    printf("\e[1;1H\e[2J");
}

bool contain(int arr[], int len, int x)
{
    for (int i = 0; i < len; i++)
    {
        if (x == arr[i])
        {
            return 1;
        }
    }
    return 0;
}

void swap(int *xp, int *yp)
{
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}

void selectionSort(int arr[], int n)
{
    int i, j, min_idx;

    for (i = 0; i < n - 1; i++)
    {
        min_idx = i;
        for (j = i + 1; j < n; j++)
            if (arr[j] < arr[min_idx])
                min_idx = j;
        swap(&arr[min_idx], &arr[i]);
    }
}

int randint(int min, int max)
{
    return min + rand() % (max + 1 - min);
}

void readArray(int a[], char name[], int length)
{
    printf("%s : ", name);
    if (length == 0)
    {
        printf("empty");
    }
    else
    {
        for (int i = 0; i < length; i++)
        {
            printf("%d ", a[i]);
        }
    }
    printf("\n");
}

void emptyArray(int a[], int length)
{
    for (int i = 0; i < length; i++)
    {
        a[i] = 0;
    }
}

void flush_stdin()
{
    while (getchar() != '\n')
    {
    }
}

void fillRandArr(int a[], char name[], int length)
{
    int min = 0;
    int max = 0;
    int ok;
    do
    {
        ok = 0;
        printf("enter the minimum value to generate:");
        ok = scanf("%d", &min);
        flush_stdin();
    } while (ok == 0);

    do
    {
        ok = 0;
        printf("enter the maximum value to generate:");
        ok = scanf("%d", &max);
        flush_stdin();
    } while (ok == 0 || min >= max);

    srand(time(NULL));
    for (int i = 0; i < length; i++)
    {
        a[i] = randint(min, max);
    }

    clearConsole();
    readArray(a, name, length);
    printf("Press any key to continue...");
    getchar();
}

void fillManArr(int a[], char name[], int length)
{
    for (int i = 0; i < length; i++)
    {
        clearConsole();
        readArray(a, name, length);
        printf("Insert the value of number #%d :", i + 1);
        scanf("%d", &a[i]);
        flush_stdin();
    }

    clearConsole();
    readArray(a, name, length);
    printf("Press any key to continue...");
    getchar();
}

void choiceFill(int a[], char name[], int length)
{
    int choice = -1;
    do
    {
        clearConsole();
        readArray(a, name, length);
        printf("choose how you want to fill the %s :\n\t 1) randomly\n\t 2) manually\n-->", name);
        scanf("%d", &choice);
        flush_stdin();

    } while (choice != 1 && choice != 2);

    if (choice == 1)
    {
        fillRandArr(a, name, length);
    }
    else
    {
        fillManArr(a, name, length);
    }
    clearConsole();
}

int *findIntersection(int a1[], int a2[], int len1, int len2, int Dup, int *NewLen)
{
    int resultDup[len1 + len2];
    int result[len1 > len2 ? len1 : len2];
    int outCountDup = 0, outCount = 0;
    bool ok;
    for (int x = 0; x < len1; x++)
    {
        ok = contain(a2, len2, a1[x]);
        if (ok)
        {
            resultDup[outCountDup] = a1[x];
            outCountDup++;
            ok = contain(result, len1 > len2 ? len1 : len2, a1[x]);
            if (!ok)
            {
                result[outCount] = a1[x];
                outCount++;
            }
        }
    }

    for (int x = 0; x < len2; x++)
    {
        ok = contain(a1, len1, a2[x]);
        if (ok)
        {
            resultDup[outCountDup] = a2[x];
            outCountDup++;
            ok = contain(result, len1 > len2 ? len1 : len2, a2[x]);
            if (!ok)
            {
                result[outCount] = a2[x];
                outCount++;
            }
        }
    }

    if (Dup == 0)
    {
        int *newArray = (int *)malloc(outCount * sizeof(int));
        for (int i = 0; i < outCount; i++)
        {
            newArray[i] = result[i];
        }
        *NewLen = outCount;
        return newArray;
    }
    else
    {
        int *newArray = (int *)malloc(outCountDup * sizeof(int));
        for (int i = 0; i < outCountDup; i++)
        {
            newArray[i] = resultDup[i];
        }
        *NewLen = outCountDup;
        return newArray;
    }
}

int *findUnion(int arr1[], int arr2[], int len1, int len2, int Dup, int *NewLen)
{
    int result[len1 + len2];
    int outCount = 0;
    bool ok;
    if (Dup == 0)
    {
        for (int i = 0; i < len1; i++)
        {
            ok = contain(result, len1 + len2, arr1[i]);
            if (!ok)
            {
                result[outCount++] = arr1[i];
            }
        }
        for (int j = 0; j < len2; j++)
        {
            ok = contain(result, len1 + len2, arr2[j]);
            if (!ok)
            {
                result[outCount++] = arr2[j];
            }
        }
    }
    else
    {
        for (int i = 0; i < len1; i++)
        {
            result[outCount++] = arr1[i];
        }

        for (int j = 0; j < len2; j++)
        {
            result[outCount++] = arr2[j];
        }
    }

    int *newArray = (int *)malloc(outCount * sizeof(int));
    for (int x = 0; x < outCount; x++)
    {
        newArray[x] = result[x];
    }
    *NewLen = outCount;
    return newArray;
}

void printMemo(int *a, int length, char name[])
{
    printf("Total size of the array %s in bytes: %lu\n", name, length * sizeof(a[0]));

    printf("Memory addresses and sizes of the elements:\n");
    for (int i = 0; i < length; i++)
    {
        printf("\t%d) Element %d - Address: %p, Size: %lu bytes\n", i, a[i], &a[i], sizeof(a[i]));
    }
    printf("---------------------------------------------------------------------\n");
}

int main()
{
    int len1 = -1;
    do
    {
        printf("enter the length of the first array:");
        scanf("%d", &len1);
        flush_stdin();
    } while (len1 <= 0);

    int array1[len1];

    emptyArray(array1, len1);
    choiceFill(array1, "Array1", len1);

    int len2 = -1;
    do
    {
        printf("enter the length of the second array:");
        scanf("%d", &len2);
        flush_stdin();
    } while (len2 <= 0);

    int array2[len2];
    emptyArray(array2, len2);
    choiceFill(array2, "Array2", len2);

    readArray(array1, "Array1", len1);
    readArray(array2, "Array2", len2);

    int interLen = 0;
    int interLenDup = 0;
    int unionLen = 0;
    int unionLenDup = 0;

    int *interArray = findIntersection(array1, array2, len1, len2, 0, &interLen);
    int *interArrayDup = findIntersection(array1, array2, len1, len2, 1, &interLenDup);
    int *unionArray = findUnion(array1, array2, len1, len2, 0, &unionLen);
    int *unionArrayDup = findUnion(array1, array2, len1, len2, 1, &unionLenDup);

    readArray(interArray, "Intersection without duplicate", interLen);
    readArray(interArrayDup, "Intersection with duplicate", interLenDup);
    readArray(unionArray, "Union without duplicate", unionLen);
    readArray(unionArrayDup, "Union with duplicate", unionLenDup);

    printf("\n\n");

    printMemo(interArray, interLen, "'Intersection without duplicate'");
    printMemo(interArrayDup, interLenDup, "'Intersection with duplicate'");
    printMemo(unionArray, unionLen, "'Union without duplicate'");
    printMemo(unionArrayDup, unionLenDup, "'Union with duplicate'");

    free(interArray);
    free(interArrayDup);
    free(unionArray);
    free(unionArrayDup);
}
