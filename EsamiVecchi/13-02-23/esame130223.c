#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

#define FILENAME "ordini.txt"
#define N_PRODOTTI 300
#define LEN_DESCRIZIONE_PRODOTTO 100
#define LEN_NOME_COGNOME_CLIENTE 70
#define LEN_INDIRIZZO_CLIENTE 70

// len stringa = caratteri utili + terminatore

typedef struct Prodotto
{
    int id_prodotto;
    char descrizione[LEN_DESCRIZIONE_PRODOTTO + 1];
    float prezzo;
    int disponibilità_residua;
    bool codice_errore;
} Prodotto;

typedef struct Ordine
{
    int id_prodotto;
    int quantità;

    struct Ordine *next;
} Ordine;

typedef struct Cliente
{
    char nome_e_cognome[LEN_NOME_COGNOME_CLIENTE + 1];
    char indirizzo[LEN_INDIRIZZO_CLIENTE + 1];
    Ordine *prodotti_ordinati;

    struct Cliente *next;
} Cliente;

int insert_prodotto(Prodotto val, Prodotto **vettore, int *size_vettore)
{
    // trova dove inserire
    int prev_size = *size_vettore;
    int index = -1;
    for (int i = 0; i < prev_size; i++)
    {
        Prodotto current = (*vettore)[i];
        if (val.id_prodotto < current.id_prodotto)
        {
            // posizione trovata
            index = i;
            break;
        }
    }
    if (index == -1)
    {
        // inserire in coda
        index = prev_size;
    }

    // aggiungi posto se non disponibile
    // ERRORE controllo
    if (prev_size % N_PRODOTTI == 0)
    {
        *vettore = (Prodotto *)realloc(*vettore, sizeof(Prodotto) * (prev_size + N_PRODOTTI));
        assert(*vettore != NULL);
    }

    (*size_vettore)++;
    // sposta successivi a index
    for (int i = prev_size; i > index; i--)
    {
        (*vettore)[i] = (*vettore)[i - 1];
    }
    // inserisci valore e ritorna indice
    (*vettore)[index] = val;
    return index;
}

Cliente *insert_cliente(Cliente *val, Cliente *lista)
{
    // inserimento in testa
    val->next = lista;
    // ritorna nuova testa
    return val;
}

Ordine *insert_ordine(Ordine *val, Ordine *lista)
{
    // inserimento in testa
    val->next = lista;
    // ritorna nuova testa
    return val;
}

Cliente *find_cliente(char *nome, char *indirizzo, Cliente *clienti)
{
    // loop lista
    Cliente *c = clienti;
    while (c != NULL)
    {
        if (strcmp(nome, c->nome_e_cognome) == 0 &&
            strcmp(indirizzo, c->indirizzo) == 0)
        {
            // cleinte trovato
            return c;
        }
        c = c->next; // passa al prossimo
    }

    return NULL; // cliente non trovato
}

Ordine *find_ordine(int id_prodotto, Ordine *ordini)
{
    // loop lista
    Ordine *o = ordini;
    while (o != NULL)
    {
        if (id_prodotto == o->id_prodotto)
        {
            // ordine trovato
            return o;
        }
        o = o->next; // passa al prossimo
    }

    return NULL; // ordine non trovato
}

Cliente *crea_cliente(char *nome, char *indirizzo, Ordine *ordini)
{
    // crea cliente
    Cliente *c = (Cliente *)malloc(sizeof(Cliente));
    assert(c != NULL);
    // riempi dati
    strcpy(c->nome_e_cognome, nome);
    strcpy(c->indirizzo, indirizzo);
    c->prodotti_ordinati = ordini;
    c->next = NULL;

    return c;
}

Ordine *crea_ordine(int id_prodotto, int quantità)
{
    // crea ordine
    Ordine *o = (Ordine *)malloc(sizeof(Ordine));
    assert(o != NULL);
    // riempi dati
    o->id_prodotto = id_prodotto;
    o->quantità = quantità;
    o->next = NULL;

    return o;
}

Prodotto *find_prodotto(int id_prodotto, Prodotto *prodotti, int size_prodotti)
{
    // binary search
    if (size_prodotti < 1)
        return NULL;
    int mid = size_prodotti / 2;
    if (id_prodotto == prodotti[mid].id_prodotto)
    {
        // trovato
        return &(prodotti[mid]);
    }
    if (size_prodotti == 1)
    {
        // non più divisibile
        return NULL;
    }
    if (id_prodotto > prodotti[mid].id_prodotto)
    {
        // cerca metà destra
        int start = mid + 1;
        return find_prodotto(id_prodotto, &(prodotti[start]), size_prodotti - start);
    }

    if (id_prodotto < prodotti[mid].id_prodotto)
    {
        // cerca metà sinistra
        return find_prodotto(id_prodotto, &(prodotti[0]), mid);
    }
}

Prodotto crea_prodotto(int id_prodotto, char *descrizione, float prezzo, int disponibilità)
{
    // assegna dati
    Prodotto p;
    p.id_prodotto = id_prodotto;
    strcpy(p.descrizione, descrizione);
    p.prezzo = prezzo;
    p.disponibilità_residua = disponibilità;
    p.codice_errore = p.disponibilità_residua == 0;
    // ritorna prodotto
    return p;
}

/*
Struttura riga:
Nome_Cognome Indirizzo id_prodotto_O1 quantità_O1 id_prodotto_O2 quantità_O2 ...
Esempio:
Mario_Rossi Via_Verdi 1 2 5 4 3 6
*/
Cliente *leggi_ordini(char *filename, Prodotto *prodotti, int size_prodotti)
{
    Cliente *clienti = NULL;
    FILE *file = fopen(filename, "r"); // apri file in lettura
    assert(file != NULL);

    // loop file
    while (!feof(file))
    {
        // leggi cliente
        char nome[LEN_NOME_COGNOME_CLIENTE + 1] = {0}, indirizzo[LEN_INDIRIZZO_CLIENTE + 1] = {0};
        fscanf(file, "%s %s", nome, indirizzo);
        if (strlen(nome) == 0 || strlen(indirizzo) == 0)
        {
            // riga vuota
            break;
        }

        // cerca cliente
        Cliente *cliente = find_cliente(nome, indirizzo, clienti);
        if (cliente == NULL)
        {
            // nuovo cliente
            cliente = crea_cliente(nome, indirizzo, NULL);
            clienti = insert_cliente(cliente, clienti);
        }

        // scorri ordini file
        char c = '\0';
        while ((c = fgetc(file)) == ' ')
        {
            // nuovo ordine
            int id_prodotto, quantità;
            fscanf(file, "%d %d", &id_prodotto, &quantità);
            // cerca ordine in lista
            Ordine *ordine = find_ordine(id_prodotto, cliente->prodotti_ordinati);
            if (ordine == NULL)
            {
                // nuovo prodotto
                ordine = crea_ordine(id_prodotto, 0);
                cliente->prodotti_ordinati = insert_ordine(ordine, cliente->prodotti_ordinati);
            }

            // prendi prodotto interessato
            Prodotto *prodotto = find_prodotto(ordine->id_prodotto, prodotti, size_prodotti);
            if (prodotto != NULL && !prodotto->codice_errore)
            {
                // prodotto esistente e disponibile
                // prendi quantità possibile
                // ERRORE variabile quantità sbagliata
                int quantità_presa = (quantità <= prodotto->disponibilità_residua ? quantità : prodotto->disponibilità_residua);
                // aggiungi e togli
                ordine->quantità += quantità_presa;
                prodotto->disponibilità_residua -= quantità_presa;
                // ricalcola codice errore
                prodotto->codice_errore = prodotto->disponibilità_residua == 0;
            }
        }
    }

    fclose(file); // chiudi file
    return clienti;
}

void insert_ordine_descrescente(Ordine *val, Ordine **lista)
{
    // scorri lista
    Ordine *current = *lista;
    Ordine *prev = NULL;
    while (current != NULL)
    {
        if (val->quantità > current->quantità)
        {
            // posizione trovata
            break;
        }

        prev = current;          // salva precedente
        current = current->next; // passa al prossimo
    }
    // inserisci nodo
    val->next = current;
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

Ordine *ordina_prodotti(Cliente *clienti)
{
    Ordine *ordini_descrescenti = NULL;
    // loop clienti
    Cliente *c = clienti;
    while (c != NULL)
    {
        // loop ordini cliente
        Ordine *o = c->prodotti_ordinati;
        while (o != NULL)
        {
            // ERRORE non ripetere più volte stesso prodotto
            // cerca ordine
            Ordine *ordine = find_ordine(o->id_prodotto, ordini_descrescenti);
            if (ordine == NULL) // non trovato
            {
                // clona ordine
                // ERRORE modificare stessa variabile
                ordine = crea_ordine(o->id_prodotto, 0);
                // inserisci in lista
                insert_ordine_descrescente(ordine, &ordini_descrescenti);
            }
            // aggiungi quantità
            ordine->quantità += o->quantità;

            o = o->next; // passa al prossimo
        }

        c = c->next; // passa al prossimo
    }

    return ordini_descrescenti;
}

void free_ordini(Ordine *ordini)
{
    if (ordini == NULL)
        return; // fine lista

    // salva prossimo
    Ordine *next = ordini->next;
    // libera memoria
    free(ordini);

    free_ordini(next); // ricorsione con prossimo
}

void free_clienti(Cliente *clienti)
{
    if (clienti == NULL)
        return; // fine lista

    // salva prossimo
    Cliente *next = clienti->next;
    // libera memoria
    free_ordini(clienti->prodotti_ordinati);
    free(clienti);

    free_clienti(next); // ricorsione con prossimo
}

// TEST
void stampa_ordini(Ordine *ordini)
{
    if (ordini == NULL)
    {
        putchar('\n');
        return; // fine lista
    }

    printf("ID prodotto: %d, Quantità: %d\n",
           ordini->id_prodotto, ordini->quantità);

    stampa_ordini(ordini->next); // ricorsione con prossimo
}
// TEST
void stampa_clienti(Cliente *clienti)
{
    if (clienti == NULL)
    {
        putchar('\n');
        return; // fine lista
    }

    printf("Nome: %s, Indirizzo: %s, Ordini:\n",
           clienti->nome_e_cognome, clienti->indirizzo);
    stampa_ordini(clienti->prodotti_ordinati);

    stampa_clienti(clienti->next); // ricorsione con prossimo
}

int main()
{
    Prodotto *prodotti = NULL;
    int size_prodotti = 0;
    insert_prodotto(((Prodotto){2, "Banane", 4.5, 3, false}), &prodotti, &size_prodotti);
    insert_prodotto(((Prodotto){0, "Mele", 1.5, 1, false}), &prodotti, &size_prodotti);
    insert_prodotto(((Prodotto){1, "Pere", 5.5, 5, false}), &prodotti, &size_prodotti);

    Cliente *clienti = leggi_ordini(FILENAME, prodotti, size_prodotti);

    // TEST
    puts("Clienti:");
    stampa_clienti(clienti);

    Ordine *ordini_descrescenti = ordina_prodotti(clienti);

    // TEST
    puts("Ordini decrescenti:");
    stampa_ordini(ordini_descrescenti);

    free_clienti(clienti);
    free_ordini(ordini_descrescenti);
    return 0;
}