#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define N_NODI 10

typedef struct Nodo
{
    int id;

    struct Nodo *next;
} Nodo;

Nodo *crea_nodo(int id)
{
    Nodo *n = (Nodo *)malloc(sizeof(Nodo));
    assert(n != NULL);
    n->id = id;
    n->next = NULL;

    return n;
}

void stampa_lista(Nodo *n)
{
    while (n != NULL)
    {
        printf("%d ", n->id);

        n = n->next;
    }
    putchar('\n');
}

void free_lista(Nodo *n)
{
    while (n != NULL)
    {
        Nodo *next = n->next;
        free(n);
        n = next;
    }
}

void invert_lista(Nodo **lista)
{
    Nodo *current = *lista;
    Nodo *prev = NULL;
    while (current != NULL)
    {
        Nodo *next = current->next;
        current->next = prev;

        prev = current;
        current = next;
    }

    *lista = prev;
}

int main()
{
    Nodo *lista = NULL;
    Nodo **current = &lista;
    for (int i = 0; i < N_NODI; i++)
    {
        *current = crea_nodo(i + 1);
        current = &((*current)->next);
    }

    stampa_lista(lista);
    invert_lista(&lista);
    stampa_lista(lista);

    free_lista(lista);
    return 0;
}