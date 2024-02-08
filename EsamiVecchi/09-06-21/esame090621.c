#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

// #define N_PRODOTTI 150
#define LEN_DESCRIZIONE_PRODOTTO 100
#define LEN_NOME_CLIENTE 70
#define LEN_INDIRIZZO 70
#define MAX_ORDINI 15
#define FILENAME "ordini.txt"

// len stringa = caratteri utili + terminatore

// TEST
#define N_PRODOTTI 3

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
} Ordine;

typedef struct OrdineCliente
{
    char nome_e_cognome[LEN_NOME_CLIENTE + 1];
    char indirizzo[LEN_INDIRIZZO + 1];
    Ordine ordini[MAX_ORDINI];
    int n_ordini; // numero di ordini nel vettore
} OrdineCliente;

typedef struct Cliente
{
    char nome_e_cognome[LEN_NOME_CLIENTE + 1];
    char indirizzo[LEN_INDIRIZZO + 1];
    Ordine *ordini;
    int n_ordini; // numero di ordini nel vettore

    struct Cliente *next;
} Cliente;

bool stesso_cliente(Cliente *c1, Cliente *c2)
{
    return (strcmp(c1->nome_e_cognome, c2->nome_e_cognome) == 0 && // stesso nome
            strcmp(c1->indirizzo, c2->indirizzo) == 0);            // stesso indirizzo
}

Prodotto *cerca_prodotto(int id_prodotto, Prodotto *prodotti)
{
    // scorri prodotti
    for (int i = 0; i < N_PRODOTTI; i++)
    {
        if (prodotti[i].id_prodotto == id_prodotto)
        {                          // stesso id
            return &(prodotti[i]); // prodotto trovato
        }
    }

    return NULL; // prodotto non trovato
}

// si presuppone che i campi del file siano divisi da spazi e che non ci siano righe vuote
Cliente *leggi_ordini(char *filename, Prodotto *prodotti)
{
    Cliente *coda_clienti = NULL;

    FILE *file = fopen(filename, "r"); // apri file in lettura
    assert(file != NULL);

    while (!feof(file))
    {
        // loop file
        OrdineCliente richiesta;
        // leggi dati fissi
        // si presuppone che nome e cognome non siano divisi da spazi, in quanto campo unico
        fscanf(file, "%s %s", richiesta.nome_e_cognome, richiesta.indirizzo);
        // leggi ordini
        richiesta.n_ordini = 0;
        for (int i = 0; i < MAX_ORDINI; i++)
        {
            char c = fgetc(file);
            if (c == ' ')
            {
                // nuova coppia
                Ordine o;
                // leggi dati
                fscanf(file, "%d %d", &(o.id_prodotto), &(o.quantità));
                richiesta.ordini[i] = o;
                // aumenta totale
                richiesta.n_ordini++;
            }
            else
                break; // fine riga
        }

        // processa ordine cliente

        // crea cliente fittizzio da ordine
        Cliente tmp;
        // assegna dati
        strcpy(tmp.nome_e_cognome, richiesta.nome_e_cognome);
        strcpy(tmp.indirizzo, richiesta.indirizzo);

        // cerca cliente
        Cliente *prev = NULL;
        Cliente *cliente_interessato = coda_clienti;
        while (cliente_interessato != NULL)
        {
            // loop lista
            if (stesso_cliente(cliente_interessato, &tmp))
            {
                // cliente trovato
                break;
            }

            // passa al prossimo
            prev = cliente_interessato;
            cliente_interessato = cliente_interessato->next;
        }
        if (cliente_interessato == NULL)
        {
            // nuovo cliente
            Cliente *nuovo_cliente = (Cliente *)malloc(sizeof(Cliente));
            assert(nuovo_cliente != NULL);
            // assegna dati
            strcpy(nuovo_cliente->nome_e_cognome, tmp.nome_e_cognome);
            strcpy(nuovo_cliente->indirizzo, tmp.indirizzo);
            nuovo_cliente->ordini = NULL;
            nuovo_cliente->n_ordini = 0;
            nuovo_cliente->next = NULL;

            // inserisci in coda
            if (prev != NULL)
            {
                prev->next = nuovo_cliente;
            }
            else
            {
                // primo cliente
                coda_clienti = nuovo_cliente;
            }

            // seleziona nuovo cliente
            cliente_interessato = nuovo_cliente;
        }

        // per ogni nuovo ordine richiesto
        for (int i = 0; i < richiesta.n_ordini; i++)
        {
            Ordine ordine_richiesto = richiesta.ordini[i];
            // prendi prodotto associato
            Prodotto *prodotto_richiesto = cerca_prodotto(ordine_richiesto.id_prodotto, prodotti);
            // controlla disponibilità
            if (prodotto_richiesto == NULL) // prodotto inesistente
            {
                continue; // salta
            }
            else if (prodotto_richiesto->codice_errore ||                                   // prodotto finito
                     ordine_richiesto.quantità > prodotto_richiesto->disponibilità_residua) // prodotto insufficiente
            {
                prodotto_richiesto->codice_errore = true; // segna errore
                continue;                                 // salta
            }

            // cerca se il prodotto è già stato ordinato
            Ordine *ordine_interessato = NULL;
            for (int j = 0; j < cliente_interessato->n_ordini; j++)
            {
                // ERRORE i invece di j
                if (cliente_interessato->ordini[j].id_prodotto == ordine_richiesto.id_prodotto)
                {
                    // prodotto trovato
                    ordine_interessato = &(cliente_interessato->ordini[j]);
                    break;
                }
            }
            if (ordine_interessato == NULL)
            {
                // nuovo prodotto
                // aggiungi cella
                cliente_interessato->ordini = (Ordine *)realloc(cliente_interessato->ordini,
                                                                sizeof(Ordine) * (cliente_interessato->n_ordini + 1));
                assert(cliente_interessato->ordini != NULL);
                // assegna ordine con quantità ancora nulla
                cliente_interessato->ordini[cliente_interessato->n_ordini] = (Ordine){ordine_richiesto.id_prodotto, 0};

                // seleziona ordine
                ordine_interessato = &(cliente_interessato->ordini[cliente_interessato->n_ordini]);
                // aumenta totale ordini
                cliente_interessato->n_ordini++;
            }

            // aggiungi e rimuovi quantità
            ordine_interessato->quantità += ordine_richiesto.quantità;
            prodotto_richiesto->disponibilità_residua -= ordine_richiesto.quantità;
        }
    }

    fclose(file); // chiudi file
    return coda_clienti;
}

void stampa_ordini(Ordine *ordini, int n_ordini)
{
    for (int i = 0; i < n_ordini; i++)
    {
        Ordine o = ordini[i];
        // stampa dati ordine
        printf("ID prodotto: %d, Quantità: %d\n", o.id_prodotto, o.quantità);
    }
}

void stampa_lista(Cliente *coda_clienti)
{
    if (coda_clienti == NULL)
        return; // fine lista

    // stampa dati
    printf("Nome: %s, Indirizzo: %s, Prodotti (%d):\n",
           coda_clienti->nome_e_cognome, coda_clienti->indirizzo, coda_clienti->n_ordini);
    // stampa ordini
    stampa_ordini(coda_clienti->ordini, coda_clienti->n_ordini);
    putchar('\n');

    stampa_lista(coda_clienti->next); // ricorsione con prossimo
}

void free_clienti(Cliente *coda_clienti)
{
    if (coda_clienti == NULL)
        return; // fine lista

    // salva prossimo
    Cliente *next = coda_clienti->next;
    // libera ordini
    free(coda_clienti->ordini);
    // libera cliente
    free(coda_clienti);

    free_clienti(next); // ricorsione con prossimo
}

// il valore degli scontrini verrà solo stampato
void calcola_scontrino_clienti(Cliente *coda_clienti, Prodotto *prodotti)
{
    while (coda_clienti != NULL)
    {
        // loop lista
        printf("Scontrino %s (Indirizzo: %s):\n", coda_clienti->nome_e_cognome, coda_clienti->indirizzo);

        float totale_scontrino = 0.0;
        // scorri ordini cliente
        for (int i = 0; i < coda_clienti->n_ordini; i++)
        {
            Ordine o = coda_clienti->ordini[i];
            // cerca prodotto
            Prodotto *p = cerca_prodotto(o.id_prodotto, prodotti);
            if (p == NULL)
            {
                // prodotto inesistente
                continue;
            }
            // calcola prezzo
            float prezzo_totale = o.quantità * p->prezzo;
            printf("Prodotto %d: %d x %.2f = %.2f\n", o.id_prodotto, o.quantità, p->prezzo, prezzo_totale);
            // aggiorna totale
            totale_scontrino += prezzo_totale;
        }

        // stampa totale
        printf("TOTALE: %.2f\n\n", totale_scontrino);

        coda_clienti = coda_clienti->next;
    }
}

// TEST
void stampa_prodotti(Prodotto *prodotti)
{
    puts("Prodotti");
    for (int i = 0; i < N_PRODOTTI; i++)
    {
        Prodotto p = prodotti[i];
        printf("ID: %d, Descrizione: %s, Prezzo: %.2f, Disponibilità: %d, Errore: %d\n",
               p.id_prodotto, p.descrizione, p.prezzo, p.disponibilità_residua, p.codice_errore);
    }
    putchar('\n');
}

int main()
{
    Prodotto prodotti[N_PRODOTTI];

    // TEST
    prodotti[0] = (Prodotto){0, "Banane", 2.5, 2, false};
    prodotti[1] = (Prodotto){1, "Mele", 4.5, 4, false};
    prodotti[2] = (Prodotto){2, "Pere", 1.5, 3, false};

    stampa_prodotti(prodotti);
    // FINE TEST

    Cliente *coda_clienti = leggi_ordini(FILENAME, prodotti);
    stampa_lista(coda_clienti);

    // TEST
    stampa_prodotti(prodotti);

    calcola_scontrino_clienti(coda_clienti, prodotti);

    free_clienti(coda_clienti);
    return 0;
}