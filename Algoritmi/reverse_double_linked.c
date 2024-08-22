#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Nodo {
  int id;

  struct Nodo *prev;
  struct Nodo *next;
} Nodo;

Nodo *crea_nodo(int id) {
  Nodo *n = (Nodo *)malloc(sizeof(Nodo));
  assert(n != NULL);
  n->id = id;
  n->prev = NULL;
  n->next = NULL;

  return n;
}

void push_nodo(Nodo *val, Nodo **lista) {
  if (val == NULL)
    return;

  Nodo *l = *lista;
  Nodo *prev = NULL;
  if (l != NULL) {
    prev = l->prev;
    l->prev = val;
  }
  if (prev != NULL) {
    prev->next = val;
  }
  val->next = l;
  val->prev = prev;

  *lista = val;
}

void stampa_lista(Nodo *n) {
  while (n != NULL) {
    printf("%d(%d %d) ", n->id, n->prev == NULL ? -1 : n->prev->id,
           n->next == NULL ? -1 : n->next->id);

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

void reverse_lista(Nodo **lista) {
  Nodo *n = *lista;
  Nodo *p = n == NULL ? NULL : n->prev;
  while (n != NULL) {
    Nodo *next = n->next;
    n->next = p;
    n->prev = next;

    p = n;
    n = next;
  }

  *lista = p;
}

int main() {
  Nodo *lista = NULL;
  for (int i = 0; i < 10; i++) {
    Nodo *n = crea_nodo(i);
    push_nodo(n, &lista);
  }

  stampa_lista(lista);
  reverse_lista(&lista);
  stampa_lista(lista);

  free_lista(lista);
  return 0;
}