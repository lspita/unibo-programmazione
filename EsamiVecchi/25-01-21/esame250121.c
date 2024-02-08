#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

#define FILENAME "ordini.txt"
#define N_PIATTI 150
#define LEN_DESCRIZIONE_PIATTO 100
#define LEN_NOME_COGNOME 70
#define MAX_ORDINI 15

#define NOME_CALCOLA_SCONTRINO "Mario" // nome cliente a cui calcolare lo scontrino tramite calcola_scontrino

typedef struct Piatto
{
    int id_piatto;
    char descrizione[LEN_DESCRIZIONE_PIATTO + 1];
    float prezzo;
} Piatto;

typedef struct Ordine
{
    int id_piatto;
    int numero_porzioni;

    struct Ordine *next;
} Ordine;

typedef struct Cliente
{
    char nome_e_cognome[LEN_NOME_COGNOME + 1];
    Ordine *ordini;

    struct Cliente *next;
} Cliente;

Piatto new_piatto(int id_piatto, char *descrizione, float prezzo)
{
    Piatto val;
    // assegna dati
    val.id_piatto = id_piatto;
    strcpy(val.descrizione, descrizione);
    val.prezzo = prezzo;

    return val;
}

// inserimento con priorità
int push_piatto(Piatto val, Piatto *vettore, int *size_vettore)
{
    int size = *size_vettore;
    // controlla se c'è spazio
    assert(size < N_PIATTI);
    // cerca posizione di inserimento
    int index = -1;
    for (int i = 0; i < size; i++)
    {
        Piatto other = vettore[i];
        if (val.id_piatto < other.id_piatto)
        {
            // posizione trovata
            index = i;
            break;
        }
    }
    if (index == -1)
    {
        // inserisci in coda
        index = size;
    }
    // sposta antecedenti
    for (int i = size; i > index; i--)
    {
        vettore[i] = vettore[i - 1];
    }
    // inserisci e ritorna posizione
    vettore[index] = val;
    (*size_vettore)++;
    return index;
}

// binary search
Piatto *find_piatto(int id_piatto, Piatto *vettore, int size)
{
    if (size == 0)
    {
        return NULL;
    }
    int mid = size / 2;
    int mid_val = vettore[mid].id_piatto;

    if (id_piatto == mid_val)
    {
        // trovato
        return &(vettore[mid]);
    }
    if (size == 1)
    {
        // non più divisibile
        return NULL;
    }
    if (id_piatto > mid_val)
    {
        // cerca a destra
        int start = mid + 1;
        return find_piatto(id_piatto, &(vettore[start]), size - start);
    }
    if (id_piatto < mid_val)
    {
        // cerca a sinistra
        return find_piatto(id_piatto, &(vettore[0]), mid);
    }
}

Ordine *new_ordine(int id_piatto, int numero_porzioni)
{
    Ordine *val = (Ordine *)malloc(sizeof(Ordine));
    assert(val != NULL);

    // assegna dati
    val->id_piatto = id_piatto;
    val->numero_porzioni = numero_porzioni;
    val->next = NULL;

    return val;
}

// inserimento in testa
void push_ordine(Ordine *val, Ordine **lista)
{
    val->next = *lista;
    *lista = val;
}

Ordine *find_ordine(int id_piatto, Ordine *lista)
{
    // loop lista
    Ordine *current = lista;
    while (current != NULL)
    {
        if (id_piatto == current->id_piatto)
        {
            // trovato
            return current;
        }

        current = current->next; // prossimo
    }
    return NULL; // non trovato
}

Cliente *new_cliente(char *nome_e_cognome, Ordine *ordini)
{
    Cliente *val = (Cliente *)malloc(sizeof(Cliente));
    assert(val != NULL);

    // assegna dati
    strcpy(val->nome_e_cognome, nome_e_cognome);
    val->ordini = ordini;
    val->next = NULL;

    return val;
}

// inserimento in testa
void push_cliente(Cliente *val, Cliente **lista)
{
    val->next = *lista;
    *lista = val;
}

Cliente *find_cliente(char *nome_e_cognome, Cliente *lista)
{
    // loop lista
    Cliente *current = lista;
    while (current != NULL)
    {
        if (strcmp(nome_e_cognome, current->nome_e_cognome) == 0)
        {
            // trovato
            return current;
        }

        current = current->next; // prossimo
    }
    return NULL; // non trovato
}
/*
Struttura riga: elementi divisi da spazi
Esempio (O = Ordine): NOME_COGNOME id_piatto_O1 numero_porzioni_O1 id_piatto_O2 numero_porzioni_O2 ...
*/
Cliente *leggi_ordini(char *filename)
{
    Cliente *clienti = NULL;
    // apri file in lettura
    FILE *file = fopen(filename, "r");
    assert(file != NULL);

    // loop file
    while (!feof(file))
    {
        // leggi nome
        char nome_e_cognome[LEN_NOME_COGNOME + 1] = {0};
        fscanf(file, "%s", nome_e_cognome);
        if (strlen(nome_e_cognome) == 0)
        {
            // riga vuota
            break;
        }

        // cerca cliente
        Cliente *c = find_cliente(nome_e_cognome, clienti);
        if (c == NULL)
        {
            // nuovo cliente
            c = new_cliente(nome_e_cognome, NULL);
            push_cliente(c, &clienti);
        }

        // scorri ordini
        for (int i = 0; i < MAX_ORDINI; i++)
        {
            char next_c = fgetc(file);
            if (next_c == ' ')
            {
                // nuovo ordine
                int id_piatto, numero_porzioni;
                fscanf(file, "%d %d", &id_piatto, &numero_porzioni);

                // cerca piatto in ordini
                Ordine *o = find_ordine(id_piatto, c->ordini);
                if (o == NULL)
                {
                    // nuovo piatto
                    o = new_ordine(id_piatto, 0);
                    push_ordine(o, &(c->ordini));
                }
                // aggiungi porzioni ordinate
                o->numero_porzioni += numero_porzioni;
            }
            else
            {
                // fine riga
                break;
            }
        }
    }

    // chiudi file
    fclose(file);
    return clienti;
}

// attenzione: la lista lo non sarà più utilizzabile in quanto divisa tra ld e lp.
void dividi_lista(Cliente *lo, Cliente **ld, Cliente **lp)
{
    static int i = 1;

    if (lo == NULL)
    {
        // fine lista
        i = 1;
        return;
    }

    Cliente *next = lo->next;
    if (i % 2 == 0)
    {
        // pari
        push_cliente(lo, lp);
    }
    else
    {
        // dispari
        push_cliente(lo, ld);
    }

    // ricorsione con prossimo
    i++;
    dividi_lista(next, ld, lp);
}

float calcola_scontrino(char *nome_e_cognome, Cliente *clienti, Piatto *piatti, int size_piatti)
{
    float totale = 0.0;
    // prendi cliente
    Cliente *c = find_cliente(nome_e_cognome, clienti);
    if (c != NULL)
    {
        // cliente trovato
        // loop ordini
        Ordine *o = c->ordini;
        while (o != NULL)
        {
            // cerca piatto
            Piatto *p = find_piatto(o->id_piatto, piatti, size_piatti);
            if (p != NULL)
            {
                // piatto trovato
                // aggiorna totale
                totale += ((float)o->numero_porzioni * p->prezzo);
            }

            o = o->next; // prossimo
        }
    }

    return totale;
}

void free_ordini(Ordine *lista)
{
    // loop lista
    while (lista != NULL)
    {
        Ordine *next = lista->next;
        // libera memoria
        free(lista);

        lista = next; // prossimo
    }
}

void free_clienti(Cliente *lista)
{
    // loop lista
    while (lista != NULL)
    {
        Cliente *next = lista->next;
        // libera memoria
        free_ordini(lista->ordini);
        free(lista);

        lista = next; // prossimo
    }
}

void print_ordini(Ordine *o)
{
    // loop lista
    while (o != NULL)
    {
        // stampa dati
        printf("Piatto: %d, Porzioni: %d\n", o->id_piatto, o->numero_porzioni);

        o = o->next;
    }
    putchar('\n');
}

void print_clienti(Cliente *c)
{
    // loop lista
    while (c != NULL)
    {
        // stampa dati
        printf("Nome: %s, Ordini:\n", c->nome_e_cognome);
        print_ordini(c->ordini);

        c = c->next;
    }
    putchar('\n');
}

void print_piatti(Piatto *piatti, int size)
{
    for (int i = 0; i < size; i++)
    {
        Piatto p = piatti[i];
        // stampa dati
        printf("Piatto: %d, Descrizione: %s, Prezzo: %.2f\n",
               p.id_piatto, p.descrizione, p.prezzo);
    }
    putchar('\n');
}

int main()
{
    Piatto piatti[N_PIATTI];
    int size_piatti = 0;
    // utilizzare push_piatto per aggiungere elementi al vettore piatti
    Piatto p;
    p = new_piatto(2, "banane", 4.5);
    push_piatto(p, piatti, &size_piatti);
    p = new_piatto(0, "mele", 5.5);
    push_piatto(p, piatti, &size_piatti);
    p = new_piatto(1, "pere", 2.5);
    push_piatto(p, piatti, &size_piatti);

    puts("PIATTI:");
    print_piatti(piatti, size_piatti);

    Cliente *clienti = leggi_ordini(FILENAME);

    puts("CLIENTI:");
    print_clienti(clienti);

    float totale = calcola_scontrino(NOME_CALCOLA_SCONTRINO, clienti, piatti, size_piatti);

    printf("TOTALE SCONTRINO %s: %.2f\n\n", NOME_CALCOLA_SCONTRINO, totale);

    Cliente *ld = NULL, *lp = NULL;
    dividi_lista(clienti, &ld, &lp);

    puts("CLIENTI DISPARI:");
    print_clienti(ld);
    puts("CLIENTI PARI:");
    print_clienti(lp);

    free_clienti(ld);
    free_clienti(lp);
    return 0;
}