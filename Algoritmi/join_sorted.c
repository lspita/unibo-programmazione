#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N_NODI 20

typedef struct Nodo {
  int id;

  struct Nodo *next;
} Nodo;

Nodo *crea_nodo(int id) {
  Nodo *n = (Nodo *)malloc(sizeof(Nodo));
  assert(n != NULL);
  n->id = id;
  n->next = NULL;

  return n;
}

void stampa_lista(Nodo *n) {
  while (n != NULL) {
    printf("%d ", n->id);

    n = n->next;
  }
  putchar('\n');
}

void free_lista(Nodo *n) {
  while (n != NULL) {
    Nodo *next = n->next;
    free(n);
    n = next;
  }
}

int randint(int min, int max) { return (rand() % (max - min + 1)) + min; }

void join_sorted_lists(Nodo *l1, Nodo *l2, Nodo **out_joined) {
  if (l1 == NULL && l2 == NULL)
    return;

  Nodo *next_l1 = l1, *next_l2 = l2;
  if (l2 != NULL && (l1 == NULL || l2->id < l1->id)) {
    next_l2 = l2->next;
    *out_joined = l2;
    (*out_joined)->next = NULL;
  } else if (l1 != NULL && (l2 == NULL || l1->id < l2->id)) {
    next_l1 = l1->next;
    *out_joined = l1;
    (*out_joined)->next = NULL;
  }
  join_sorted_lists(next_l1, next_l2, &((*out_joined)->next));
}

int main() {
  srand(time(NULL));

  Nodo *lista1 = NULL;
  Nodo **current1 = &(lista1);
  Nodo *lista2 = NULL;
  Nodo **current2 = &(lista2);
  for (int i = 0; i < N_NODI; i++) {
    Nodo *n = crea_nodo(i + 1);

    int choice = randint(1, 2);
    if (choice == 1) {
      *current1 = n;
      current1 = &((*current1)->next);
    } else {
      *current2 = n;
      current2 = &((*current2)->next);
    }
  }

  stampa_lista(lista1);
  stampa_lista(lista2);
  Nodo *joined = NULL;
  join_sorted_lists(lista1, lista2, &joined);
  stampa_lista(joined);

  free_lista(joined);
  return 0;
}