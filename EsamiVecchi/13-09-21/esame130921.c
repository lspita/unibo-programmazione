#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

#define SOGLIA_KG_PACCO_LEGGERO 10 // soglia di kg sotto la quale un pacco è leggero
#define REALLOC_JUMP 10            // di quanti elementi aumentare il vettore dinamico ogni volta
#define UNDEFINED_ID_CAMION -1     // id camion errore

typedef struct Pacco
{
    int id_pacco;
    int peso_kg;
    float valore;

    struct Pacco *next;
} Pacco;

typedef struct PaccoDogana
{
    int id_camion;
    int id_pacco;
    int peso_kg;

    struct PaccoDogana *next;
} PaccoDogana;

typedef struct Camion
{
    int id_camion;
    Pacco *pila_pacchi;
    int peso_totale;
    float valore_totale;
} Camion;

// struttura wrapper per vettore dinamico
typedef struct CodaCamion
{
    Camion *data;
    int size;
} CodaCamion;

void push_camion(Camion val, CodaCamion *coda)
{
    // controllo spazio e aggiungi se necessario
    if (coda->size % REALLOC_JUMP == 0)
    {
        // aggiungi spazio
        coda->data = (Camion *)realloc(coda->data, sizeof(Camion) * (coda->size + REALLOC_JUMP));
        assert(coda->data != NULL);
    }
    // aggiungi elemento
    coda->data[coda->size] = val;
    coda->size++;
}

bool is_empty_camion(CodaCamion *coda)
{
    // true se non ci sono camion rimanenti
    return coda->size == 0 || coda->data == NULL;
}

Camion *peek_camion(CodaCamion *coda)
{
    if (is_empty_camion(coda))
    {
        // camion finito
        return NULL;
    }
    else
    {
        // puntatore a prossimo camion
        return &(coda->data[0]);
    }
}

void free_pacchi(Pacco *p)
{
    while (p != NULL)
    {
        Pacco *next = p->next;
        free(p);
        p = next;
    }
}

void free_pacchi_dogana(PaccoDogana *pd)
{
    while (pd != NULL)
    {
        PaccoDogana *next = pd->next;
        free(pd);
        pd = next;
    }
}

void free_coda(CodaCamion *coda)
{
    // ERRORE no free pacchi
    for (int i = 0; i < coda->size; i++)
    {
        free_pacchi(coda->data[i].pila_pacchi);
    }
    free(coda->data);
    coda->size = 0;
}

Camion pop_camion(CodaCamion *coda)
{
    // prendi prossimo valore
    // ERRORE peek non utilizzato
    Camion *next_val = peek_camion(coda);
    assert(next_val != NULL);
    Camion val = *next_val;

    // sposta tutti gli elementi di 1 a sinistra
    for (int i = 0; i < coda->size - 1; i++)
    {
        coda->data[i] = coda->data[i + 1];
    }
    // riduci vettore
    coda->size--;
    if (coda->size == 0)
    {
        // libera memoria e resetta vettore
        free_coda(coda);
    }
    else if (coda->size % REALLOC_JUMP == 0)
    {
        // rimuovi celle in eccesso
        coda->data = (Camion *)realloc(coda->data, sizeof(Camion) * (coda->size));
        assert(coda->data != NULL);
    }
    // ritorna elemento estratto
    return val;
}

void push_pacco(Pacco *val, Pacco **pila)
{
    // inserimento in testa
    val->next = *pila;
    *pila = val;
}

void push_pacco_dogana(PaccoDogana *val, PaccoDogana **pila)
{
    // inserimento in testa
    val->next = *pila;
    *pila = val;
}

PaccoDogana *crea_pacco_dogana(int id_pacco, int id_camion, int peso_kg)
{
    // crea pacco dogana
    PaccoDogana *pd = (PaccoDogana *)malloc(sizeof(PaccoDogana));
    assert(pd != NULL);
    // assegna valori
    pd->id_camion = id_camion;
    pd->id_pacco = id_pacco;
    pd->peso_kg = peso_kg;
    pd->next = NULL;

    return pd;
}

void arrivo_in_dogana(Camion c, CodaCamion *coda,
                      PaccoDogana **out_pacchi_leggeri, PaccoDogana **out_pacchi_pesanti)
{
    // resetta campi
    c.peso_totale = 0;
    c.valore_totale = 0.0;
    // scorri pacchi
    Pacco *p = c.pila_pacchi;
    while (p != NULL)
    {
        // aggiorna totali
        c.peso_totale += p->peso_kg;
        c.valore_totale += p->valore;
        // crea versione dogana del pacco
        PaccoDogana *pd = crea_pacco_dogana(p->id_pacco, c.id_camion, p->peso_kg);
        // aggiungi a lista corrispondente
        if (pd->peso_kg <= SOGLIA_KG_PACCO_LEGGERO)
        {
            push_pacco_dogana(pd, out_pacchi_leggeri);
        }
        else
        {
            push_pacco_dogana(pd, out_pacchi_pesanti);
        }

        p = p->next; // prossimo pacco
    }

    // inserisci camion in coda
    push_camion(c, coda);
}

// struttura di supporto per funzione statistica_pacchi_pesanti
typedef struct CamionPacchi
{
    int id_camion;
    int n_pacchi;

    struct CamionPacchi *next;
} CamionPacchi;

CamionPacchi *find_camion_pacchi(int id_camion, CamionPacchi *lista)
{
    // loop lista
    CamionPacchi *cp = lista;
    while (cp != NULL)
    {
        if (cp->id_camion == id_camion)
        {
            // trovato
            return cp;
        }

        cp = cp->next; // prossimo
    }

    return NULL; // non trovato
}

CamionPacchi *crea_camion_pacchi(int id_camion, int n_pacchi)
{
    // crea nodo
    CamionPacchi *cp = (CamionPacchi *)malloc(sizeof(CamionPacchi));
    assert(cp != NULL);
    // assegna dati
    cp->id_camion = id_camion;
    cp->n_pacchi = n_pacchi;

    return cp;
}

void push_camion_pacchi(CamionPacchi *val, CamionPacchi **lista)
{
    // inserimento in testa
    val->next = *lista;
    *lista = val;
}

void free_camion_pacchi(CamionPacchi *cp)
{
    // loop lista
    while (cp != NULL)
    {
        // salva prossimo e libera memoria
        CamionPacchi *next = cp->next;
        free(cp);

        cp = next; // passa a prossimo
    }
}

int statistica_pacchi_pesanti(PaccoDogana *pacchi_pesanti)
{
    CamionPacchi *max_cp = NULL;
    CamionPacchi *camion_pacchi = NULL;
    // loop pacchi dogana
    PaccoDogana *pd = pacchi_pesanti;
    while (pd != NULL)
    {
        // trova camion in lista
        CamionPacchi *cp = find_camion_pacchi(pd->id_camion, camion_pacchi);
        if (cp == NULL)
        {
            // nuovo camion
            cp = crea_camion_pacchi(pd->id_camion, 0);
            push_camion_pacchi(cp, &camion_pacchi);
        }
        // aumenta totale
        cp->n_pacchi++;
        // controlla nuovo vincitore
        if (max_cp == NULL || max_cp->n_pacchi < cp->n_pacchi)
        {
            max_cp = cp;
        }

        pd = pd->next; // passa al prossimo
    }

    if (max_cp == NULL)
    {
        // nessun vincitore
        return UNDEFINED_ID_CAMION;
    }
    else
    {
        // salva id
        int id = max_cp->id_camion;
        // libera lista
        free_camion_pacchi(camion_pacchi);
        // ritorna id
        return id;
    }
}

void unisci_pacchi(PaccoDogana *leggeri, PaccoDogana *pesanti,
                   PaccoDogana **out_pacchi)
{
    if (leggeri == NULL && pesanti == NULL)
    {
        // fine lista
        return;
    }
    PaccoDogana *next_l = leggeri, *next_p = pesanti;
    // aggiungi leggero
    if (leggeri != NULL)
    {
        // ERRORE salva prima di inserire
        // salva prossimo
        next_l = leggeri->next;
        // aggiungi in lista
        push_pacco_dogana(leggeri, out_pacchi);
    }
    if (pesanti != NULL)
    {
        // salva prossimo
        next_p = pesanti->next;
        // aggiungi in lista
        push_pacco_dogana(pesanti, out_pacchi);
    }
    // ricorsione con prossimi
    unisci_pacchi(next_l, next_p, out_pacchi);
}

// TEST
Pacco *crea_pacco(int id_pacco, int peso_kg, float valore)
{
    Pacco *p = (Pacco *)malloc(sizeof(Pacco));
    assert(p != NULL);
    p->id_pacco = id_pacco;
    p->peso_kg = peso_kg;
    p->valore = valore;
    p->next = NULL;

    return p;
}

// TEST
void stampa_pacchi(Pacco *p)
{
    while (p != NULL)
    {
        printf("Pacco: %d, Peso: %d, Valore: %.2f\n",
               p->id_pacco, p->peso_kg, p->valore);

        p = p->next;
    }
    putchar('\n');
}

// TEST
void stampa_pacchi_dogana(PaccoDogana *pd)
{
    while (pd != NULL)
    {
        printf("Pacco: %d, Camion: %d, Peso: %d\n",
               pd->id_pacco, pd->id_camion, pd->peso_kg);

        pd = pd->next;
    }
    putchar('\n');
}

// TEST
void stampa_camion(Camion c)
{
    printf("Camion: %d, Peso: %d, Valore: %.2f, Pacchi:\n",
           c.id_camion, c.peso_totale, c.valore_totale);
    stampa_pacchi(c.pila_pacchi);
}

// TEST
void stampa_coda(CodaCamion *coda)
{
    for (int i = 0; i < coda->size; i++)
    {
        Camion c = coda->data[i];
        stampa_camion(c);
    }
}

void stampa_coda_pop(CodaCamion *coda)
{
    while (!is_empty_camion(coda))
    {
        Camion c = pop_camion(coda);
        stampa_camion(c);
    }

    if (is_empty_camion(coda))
    {
        puts("CODA SVUOTATA CON SUCCESSO");
    }
    else
    {
        puts("CODA NON SVUOTATA COMPLETAMENTE");
    }
}

// TEST
int main()
{
    CodaCamion coda = {NULL, 0};
    PaccoDogana *pacchi_pesanti = NULL, *pacchi_leggeri = NULL;

    Camion c = {0, NULL, 0, 0};
    Pacco *p = NULL;

    c.id_camion = 0;
    c.pila_pacchi = NULL;
    p = crea_pacco(0, 5, 4.5);
    push_pacco(p, &c.pila_pacchi);
    p = crea_pacco(1, 15, 7.5);
    push_pacco(p, &c.pila_pacchi);
    p = crea_pacco(2, 7, 5.5);
    push_pacco(p, &c.pila_pacchi);
    arrivo_in_dogana(c, &coda, &pacchi_leggeri, &pacchi_pesanti);

    c.id_camion = 1;
    c.pila_pacchi = NULL;
    p = crea_pacco(3, 20, 10.5);
    push_pacco(p, &c.pila_pacchi);
    p = crea_pacco(4, 15, 7.5);
    push_pacco(p, &c.pila_pacchi);
    arrivo_in_dogana(c, &coda, &pacchi_leggeri, &pacchi_pesanti);

    puts("Camion:");
    stampa_coda(&coda);

    puts("Pacchi leggeri:");
    stampa_pacchi_dogana(pacchi_leggeri);
    puts("Pacchi pesanti:");
    stampa_pacchi_dogana(pacchi_pesanti);

    int id_camion_statistica = statistica_pacchi_pesanti(pacchi_pesanti);
    printf("Camion con più pacchi pesanti: %d\n\n", id_camion_statistica);

    PaccoDogana *pacchi_dogana = NULL;
    unisci_pacchi(pacchi_leggeri, pacchi_pesanti, &pacchi_dogana);
    puts("Pacchi dogana:");
    stampa_pacchi_dogana(pacchi_dogana);

    free_coda(&coda);
    free_pacchi_dogana(pacchi_dogana);
    return 0;
}