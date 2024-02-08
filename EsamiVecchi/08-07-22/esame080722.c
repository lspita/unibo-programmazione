#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

// TEST
#define ID_STAMPA_ESAURIMENTO 0
#define ID_ELIMINA 2
#define ID_DESTINAZIONE 1

typedef struct prodotto_listino
{
    int codice_prodotto;
    int codice_fornitore;
    float prezzo_unitario;
    int quantità_minima;
} prodotto_listino;

typedef struct prodotto_supermercato
{
    int codice_prodotto;
    int codice_fornitore;
    int quantità_presente;

    struct prodotto_supermercato *next;
} prodotto_supermercato;

typedef struct supermercato
{
    int codice_supermercato;
    prodotto_supermercato *prodotti;

    struct supermercato *next;
} supermercato;

void insert_ordinato_prodotto_listino(prodotto_listino val, prodotto_listino **vettore, int *size_vettore)
{
    if (vettore == NULL || size_vettore == NULL)
        return;
    // aggiungi cella
    *vettore = (prodotto_listino *)realloc(*vettore, sizeof(prodotto_listino) * ((*size_vettore) + 1));
    assert(*vettore != NULL);

    int prev_size = *size_vettore;
    // cerca posizione di inserimento
    int index = -1;
    for (int i = 0; i < prev_size; i++)
    {
        if (val.codice_prodotto < (*vettore)[i].codice_prodotto ||   // prodotto maggiore
            (val.codice_prodotto == (*vettore)[i].codice_prodotto && // prodotto uguale ma fornitore maggiore
             val.codice_fornitore < (*vettore)[i].codice_fornitore))
        {
            index = i; // salva index
            break;     // esci dal ciclo
        }
    }
    if (index == -1)
    {
        // inserisci in coda
        index = prev_size;
    }
    // sposta di 1 per liberare spazio
    for (int i = prev_size; i > index; i--)
    {
        (*vettore)[i] = (*vettore)[i - 1];
    }
    // aggiungi a indice corretto
    (*vettore)[index] = val;
    (*size_vettore)++;
}

void insert_ordinato_prodotto_supermercato(prodotto_supermercato *val, prodotto_supermercato **lista)
{
    prodotto_supermercato *prev = NULL;
    prodotto_supermercato *selected = *lista;
    while (selected != NULL) // scorri lista
    {
        if (val->codice_prodotto < selected->codice_prodotto ||   // prodotto maggiore
            (val->codice_prodotto == selected->codice_prodotto && // prodotto uguale ma fornitore maggiore
             val->codice_fornitore == selected->codice_fornitore))
        {
            break;
        }

        prev = selected;           // salva come precedente
        selected = selected->next; // prossimo elemento
    }
    // inserisci in mezzo ai selezionati
    val->next = selected;
    if (prev != NULL)
    {
        prev->next = val;
    }
    else
    {
        // nuova testa
        *lista = val;
    }
}

void insert_supermercato(supermercato *val, supermercato **lista)
{
    // inserimento in testa
    val->next = *lista;
    *lista = val;
}

void free_prodotti_supermercato(prodotto_supermercato *lista)
{
    while (lista != NULL)
    {                                              // loop lista
        prodotto_supermercato *next = lista->next; // salva prossimo
        free(lista);                               // libera corrente
        lista = next;                              // passa al prossimo
    }
}

void free_supermercati(supermercato *lista)
{
    while (lista != NULL)
    {                                     // loop lista
        supermercato *next = lista->next; // salva prossimo

        free_prodotti_supermercato(lista->prodotti); // libera prodotti
        free(lista);                                 // libera corrente

        lista = next; // passa al prossimo
    }
}

prodotto_supermercato *cerca_prodotto_supermercato(int codice_prodotto, int codice_fornitore, prodotto_supermercato *prodotti)
{
    // loop prodotti
    while (prodotti != NULL)
    {
        if (prodotti->codice_prodotto == codice_prodotto && prodotti->codice_fornitore == codice_fornitore)
        {
            return prodotti; // prodotto presente
        }

        prodotti = prodotti->next; // passa al prossimo
    }

    return NULL; // non trovato
}

void chiudi_supermercato(int codice_supermercato_elimina, int codice_supermercato_destinazione, supermercato **supermercati)
{
    supermercato *prev_elimina = NULL;
    supermercato *s_elimina = NULL;
    supermercato *s_destinazione = NULL;

    supermercato *current = *supermercati;
    while (current != NULL) // scorri lista
    {
        if (current->codice_supermercato == codice_supermercato_elimina)
        {
            // supermercato da eliminare
            s_elimina = current;
        }
        else if (current->codice_supermercato == codice_supermercato_destinazione)
        {
            // supermercato destinazione prodotti
            s_destinazione = current;
        }

        if (s_elimina != NULL && s_destinazione != NULL)
        {
            // trovati tutti i supermercati interessati
            break;
        }
        if (s_elimina == NULL)
        {
            // salva precedente a quello da eliminare
            prev_elimina = current;
        }
        current = current->next; // prossimo elemento
    }
    prodotto_supermercato *prodotti_copia = NULL;
    if (s_elimina != NULL)
    {
        // riattacca lista
        if (prev_elimina != NULL)
        {
            prev_elimina->next = s_elimina->next;
        }
        else
        {
            // nuova testa
            *supermercati = s_elimina->next;
        }
        // salva prodotti
        prodotti_copia = s_elimina->prodotti;
        // elimina supermercato
        free(s_elimina);
    }

    // sposta prodotti
    if (s_destinazione != NULL)
    {
        while (prodotti_copia != NULL) // loop lista
        {
            prodotto_supermercato *next = prodotti_copia->next; // salva prossimo
            prodotto_supermercato *p = cerca_prodotto_supermercato(prodotti_copia->codice_prodotto, prodotti_copia->codice_fornitore, s_destinazione->prodotti);
            if (p == NULL)
            {
                // nuovo prodotto
                insert_ordinato_prodotto_supermercato(prodotti_copia, &(s_destinazione->prodotti));
            }
            else
            {
                // aggiungi quantità
                p->quantità_presente += prodotti_copia->quantità_presente;
            }
            // inserisci prodotto in lista destinazione

            prodotti_copia = next; // passa al prossimo
        }
    }
}

prodotto_listino *cerca_prodotto_in_listino(int codice_prodotto, int codice_fornitore, prodotto_listino *listino, int size_listino)
{
    // scorri listino
    for (int i = 0; i < size_listino; i++)
    {
        if (listino[i].codice_prodotto == codice_prodotto && listino[i].codice_fornitore == codice_fornitore)
        {
            // prodotto trovato
            return &(listino[i]);
        }
    }
    return NULL; // non trovato
}

void stampa_prodotti_in_esaurimento(int codice_supermercato, supermercato *supermercati, prodotto_listino *listino, int size_listino)
{
    supermercato *s = NULL;
    while (supermercati != NULL)
    {
        // loop lista
        if (supermercati->codice_supermercato == codice_supermercato)
        {
            // salva supermercato interessato
            s = supermercati;
            break;
        }

        supermercati = supermercati->next; // passa al prossimo
    }
    if (s == NULL)
    {
        printf("Supermercato non trovato\n");
        return;
    }
    // scorri prodotti supermercato
    prodotto_supermercato *current = s->prodotti;
    while (current != NULL)
    {
        // loop lista

        // cerca prodotto nel listino
        prodotto_listino *pl = cerca_prodotto_in_listino(current->codice_prodotto, current->codice_fornitore, listino, size_listino);
        if (pl != NULL && current->quantità_presente < pl->quantità_minima) // prodotto in listino e in esaurimento
        {
            // stampa info prodotto in esaurimento
            printf("ID prodotto: %d, ID fornitore %d, Prezzo unitario: %.2f, Quantità presente: %d, Quantità minima: %d\n",
                   current->codice_prodotto, current->codice_fornitore, pl->prezzo_unitario, current->quantità_presente, pl->quantità_minima);
        }

        current = current->next; // passa al prossimo
    }
}

int miglior_supermercato(supermercato *supermercati, prodotto_listino *listino, int size_listino)
{
    int codice_migliore = -1;
    float valore_migliore = -1.0;

    // loop supermercati
    while (supermercati != NULL)
    {
        float valore_scontrino = 0.0;
        // loop prodotti
        prodotto_supermercato *prodotti = supermercati->prodotti;
        while (prodotti != NULL)
        {
            // prendi equivalente in listino
            prodotto_listino *pl = cerca_prodotto_in_listino(prodotti->codice_prodotto, prodotti->codice_fornitore, listino, size_listino);
            if (pl != NULL) // prodotto in listino
            {
                valore_scontrino += (pl->prezzo_unitario * prodotti->quantità_presente);
            }

            prodotti = prodotti->next; // passa al prossimo
        }

        // TEST
        printf("Valore supermercato %d: %.2f\n", supermercati->codice_supermercato, valore_scontrino);

        if (valore_migliore < valore_scontrino)
        {
            // nuovo migliore
            valore_migliore = valore_scontrino;
            codice_migliore = supermercati->codice_supermercato;
        }

        supermercati = supermercati->next; // passa al prossimo
    }

    return codice_migliore;
}

// TEST
void stampa_listino(prodotto_listino *listino, int size_listino)
{
    for (int i = 0; i < size_listino; i++)
    {
        // stampa dati prodotto
        printf("ID prodotto: %d, ID fornitore %d, Prezzo unitario: %.2f, Quantità minima: %d\n",
               listino[i].codice_prodotto, listino[i].codice_fornitore,
               listino[i].prezzo_unitario, listino[i].quantità_minima);
    }
    putchar('\n');
}

// TEST
prodotto_supermercato *crea_prodotto_supermercato(prodotto_supermercato val, prodotto_supermercato **lista)
{
    // crea nuovo prodotto
    prodotto_supermercato *p = (prodotto_supermercato *)malloc(sizeof(prodotto_supermercato));
    assert(p != NULL);
    // copia dati
    *p = val;
    // aggiungi in lista
    insert_ordinato_prodotto_supermercato(p, lista);
    return p;
}

// TEST
supermercato *crea_supermercato(int codice_supermercato, supermercato **lista)
{
    // crea supermercato
    supermercato *s = (supermercato *)malloc(sizeof(supermercato));
    assert(s != NULL);
    // copia dati
    s->codice_supermercato = codice_supermercato;
    // inserisci in lista
    insert_supermercato(s, lista);
    return s;
}

// TEST
void stampa_prodotti_supermercato(prodotto_supermercato *prodotti)
{
    while (prodotti != NULL)
    {
        // loop prodotti
        printf("ID prodotto: %d, ID fornitore: %d, Quantità presente: %d\n",
               prodotti->codice_prodotto, prodotti->codice_fornitore, prodotti->quantità_presente);

        prodotti = prodotti->next; // passa al prossimo
    }
    putchar('\n');
}

// TEST
void stampa_supermercati(supermercato *supermercati)
{
    while (supermercati != NULL)
    {
        // loop supermercati
        printf("ID supermercato: %d, Prodotti:\n", supermercati->codice_supermercato);
        stampa_prodotti_supermercato(supermercati->prodotti);

        supermercati = supermercati->next; // passa al prossimo
    }
}

int main()
{
    // inizializza listino
    prodotto_listino *listino = NULL;
    int size_listino = 0;
    supermercato *supermercati = NULL;

    // TEST
    // dati test
    insert_ordinato_prodotto_listino(((prodotto_listino){2, 0, 2.5, 2}), &listino, &size_listino);
    insert_ordinato_prodotto_listino(((prodotto_listino){2, 1, 4.5, 1}), &listino, &size_listino);
    insert_ordinato_prodotto_listino(((prodotto_listino){0, 1, 1.5, 3}), &listino, &size_listino);
    insert_ordinato_prodotto_listino(((prodotto_listino){1, 0, 2.0, 5}), &listino, &size_listino);
    puts("Listino:");
    stampa_listino(listino, size_listino);

    supermercato *s = NULL;
    // dati test
    s = crea_supermercato(0, &supermercati);
    crea_prodotto_supermercato(((prodotto_supermercato){2, 1, 5}), &(s->prodotti));
    crea_prodotto_supermercato(((prodotto_supermercato){2, 0, 1}), &(s->prodotti));
    crea_prodotto_supermercato(((prodotto_supermercato){1, 0, 3}), &(s->prodotti));

    s = crea_supermercato(1, &supermercati);
    crea_prodotto_supermercato(((prodotto_supermercato){2, 1, 0}), &(s->prodotti));
    crea_prodotto_supermercato(((prodotto_supermercato){0, 1, 3}), &(s->prodotti));
    crea_prodotto_supermercato(((prodotto_supermercato){2, 0, 2}), &(s->prodotti));

    s = crea_supermercato(2, &supermercati);
    crea_prodotto_supermercato(((prodotto_supermercato){2, 1, 2}), &(s->prodotti));
    crea_prodotto_supermercato(((prodotto_supermercato){1, 0, 3}), &(s->prodotti));

    puts("Supermercati:");
    stampa_supermercati(supermercati);
    // FINE TEST

    chiudi_supermercato(ID_ELIMINA, ID_DESTINAZIONE, &supermercati);
    // TEST
    puts("Supermercati:");
    stampa_supermercati(supermercati);

    // TEST
    printf("Prodotti in esaurimento supermercato %d:\n", ID_STAMPA_ESAURIMENTO);

    stampa_prodotti_in_esaurimento(ID_STAMPA_ESAURIMENTO, supermercati, listino, size_listino);
    putchar('\n');

    int id_miglior_supermercato = miglior_supermercato(supermercati, listino, size_listino);
    putchar('\n');
    if (id_miglior_supermercato == -1)
    {
        puts("Nessun miglior supermercato presente");
    }
    else
    {
        printf("Miglior supermercato: %d\n", id_miglior_supermercato);
    }

    free(listino);
    free_supermercati(supermercati);
    return 0;
}