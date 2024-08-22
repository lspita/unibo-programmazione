#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Nodo {
  char *id;

  struct Nodo *figli;
  struct Nodo *next;
} Nodo;

Nodo *crea_nodo(char *id, Nodo *figli) {
  Nodo *n = (Nodo *)malloc(sizeof(Nodo));
  assert(n != NULL);
  n->id = id;
  n->figli = figli;
  n->next = NULL;

  return n;
}

void stampa_lista(Nodo *n) {
  while (n != NULL) {
    printf("%s", n->id);
    Nodo *f = n->figli;
    if (f != NULL) {
      printf("->(");
      stampa_lista(f);
      printf(")");
    }

    n = n->next;
    if (n != NULL) {
      printf(", ");
    }
  }
}

void free_lista(Nodo *n) {
  while (n != NULL) {
    Nodo *next = n->next;
    free(n->figli);
    free(n);
    n = next;
  }
}

void flat_lista(Nodo *n) {
  while (n != NULL) {
    Nodo *next = n->next;
    flat_lista(n->figli);
    n->next = n->figli;
    Nodo *last = n;
    while (n->figli != NULL) {
      last = n->figli;
      n->figli = n->figli->next;
    }
    last->next = next;
    n = next;
  }
}

int main() {
  Nodo *lista = crea_nodo("1", NULL);
  lista->figli = crea_nodo("1.1", NULL);
  lista->figli->figli = crea_nodo("1.1.1", NULL);
  lista->figli->figli->next = crea_nodo("1.1.2", NULL);
  lista->figli->next = crea_nodo("1.2", NULL);
  lista->next = crea_nodo("2", NULL);

  stampa_lista(lista);
  putchar('\n');
  flat_lista(lista);
  stampa_lista(lista);
  putchar('\n');

  free_lista(lista);
  return 0;
}