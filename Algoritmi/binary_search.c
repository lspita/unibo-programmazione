#include <stdio.h>

#define SIZE 20
#define SEARCH 0

typedef struct Elemento
{
    int id;
} Elemento;

Elemento *binary_search(int id, Elemento *vettore, int size)
{
    if (size < 1)
        return;
    // trova metà e valore corrispondente
    int mid = size / 2;
    int mid_id = vettore[mid].id;

    if (mid_id == id)
    {
        // trovato
        return &(vettore[mid]);
    }
    else if (size == 1)
    {
        // nessuna divisione rimasta
        return NULL;
    }
    else if (mid_id > id)
    {
        // cerca metà sinistra
        return binary_search(id, &(vettore[0]), mid);
    }
    else if (mid_id < id)
    {
        // cerca metà destra
        return binary_search(id, &(vettore[mid + 1]), size - (mid + 1));
    }
}

int main()
{
    Elemento vettore[SIZE];
    for (int i = 0; i < SIZE; i++)
    {
        int id = i + 1;
        vettore[i] = (Elemento){id};
        printf("%d ", id);
    }
    putchar('\n');

    printf("Cercando %d\n", SEARCH);
    Elemento *e = binary_search(SEARCH, vettore, SIZE);
    if (e == NULL)
    {
        printf("Non trovato\n");
    }
    else
    {
        printf("Trovato (%d)\n", e->id);
    }
}