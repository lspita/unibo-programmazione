#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define N_NODI 10

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

Nodo *middle_lista(Nodo *lista) {
  Nodo *slow = lista;
  Nodo *fast = lista;
  while (fast != NULL) {
    fast = fast->next == NULL ? fast->next : fast->next->next;
    if (fast != NULL) {
      slow = slow->next;
    }
  }

  return slow;
}

int main() {
  Nodo *lista = NULL;
  Nodo **current = &lista;
  for (int i = 0; i < N_NODI; i++) {
    *current = crea_nodo(i + 1);
    current = &((*current)->next);
  }

  stampa_lista(lista);
  Nodo *middle = middle_lista(lista);
  printf("Middle: %d\n", middle == NULL ? -1 : middle->id);

  free_lista(lista);
  return 0;
}