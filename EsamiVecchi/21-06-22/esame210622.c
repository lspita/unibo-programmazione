#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

#define FILENAME "appuntamenti.txt"
#define LEN_NOME_COGNOME_AGENTE 70
#define LEN_ID_AGENTE 10
#define LEN_ID_CLIENTE 10
#define LEN_CODICE_RICHIESTA_PRESTAZIONE 15
#define N_AGENTI 100

#define NOME_AGENTE_DEFAULT "NONAME" // nome agente assegnato se l'id non viene trovato in id_nomi_agente
#define SIZE_CLASSIFICA 3            // numero di posizioni in classifica di premio_agente_anno

typedef struct Appuntamento
{
    char id_cliente[LEN_ID_CLIENTE + 1];
    char codice_richiesta_prestazione[LEN_CODICE_RICHIESTA_PRESTAZIONE + 1];

    struct Appuntamento *next;
} Appuntamento;

typedef struct Agente
{
    char nome_cognome_agente[LEN_NOME_COGNOME_AGENTE + 1];
    char id_agente[LEN_ID_AGENTE + 1];
    int n_clienti_agente;
    Appuntamento *appuntamenti;

    struct Agente *next;
} Agente;

typedef struct RecordFile
{
    char id_cliente[LEN_ID_CLIENTE + 1];
    char id_agente[LEN_ID_AGENTE + 1];
    char codice_richiesta_prestazione[LEN_CODICE_RICHIESTA_PRESTAZIONE + 1];
} RecordFile;

typedef struct AgenteNome
{
    char nome_cognome_agente[LEN_NOME_COGNOME_AGENTE + 1];
    char id_agente[LEN_ID_AGENTE + 1];
} AgenteNome;

AgenteNome new_agente_nome(char *nome_cognome, char *id)
{
    AgenteNome an;
    // assegna dati
    strcpy(an.nome_cognome_agente, nome_cognome);
    strcpy(an.id_agente, id);

    return an;
}

// inserimento con priorità
int push_agente_nome(AgenteNome val, AgenteNome *vettore, int *size_vettore)
{
    int size = *size_vettore;
    // controlla spazio disponibile
    assert(size < N_AGENTI);
    // cerca posizione di inserimento
    int index = -1;
    for (int i = 0; i < size; i++)
    {
        AgenteNome other = vettore[i];
        if (strcmp(val.id_agente, other.id_agente) < 0)
        {
            // trovata
            index = i;
            break;
        }
    }
    if (index == -1)
    {
        // in coda
        index = size;
    }
    // sposta antecedenti
    for (int i = size; i > index; i--)
    {
        vettore[i] = vettore[i - 1];
    }
    // inserisci e ritorna indice
    vettore[index] = val;
    (*size_vettore)++;
    return index;
}

AgenteNome *find_agente_nome(char *id_agente, AgenteNome *vettore, int size)
{
    if (size == 0)
        return NULL;

    int mid = size / 2;
    int cmp_val = strcmp(id_agente, vettore[mid].id_agente);

    if (cmp_val == 0)
    {
        // trovato
        return &(vettore[mid]);
    }
    if (size == 1)
    {
        // non più divisibile
        return NULL;
    }
    if (cmp_val > 0)
    {
        // cerca a destra
        int start = mid + 1;
        return find_agente_nome(id_agente, &(vettore[start]), size - start);
    }
    if (cmp_val < 0)
    {
        // cerca a sinistra
        return find_agente_nome(id_agente, &(vettore[0]), mid);
    }
}

Appuntamento *new_appuntamento(char *id_cliente, char *codice_richiesta)
{
    Appuntamento *val = (Appuntamento *)malloc(sizeof(Appuntamento));
    assert(val != NULL);
    // assegna dati
    strcpy(val->id_cliente, id_cliente);
    strcpy(val->codice_richiesta_prestazione, codice_richiesta);

    return val;
}

// inserimento con priorità
void push_appuntamento(Appuntamento *val, Appuntamento **lista)
{
    if (val == NULL)
        return;

    // cerca posizione in lista
    Appuntamento *current = *lista;
    Appuntamento *prev = NULL;
    while (current != NULL)
    {
        if (strcmp(val->id_cliente, current->id_cliente) < 0)
        {
            // trovato
            break;
        }

        // prossimo
        prev = current;
        current = current->next;
    }
    // inserisci in mezzo
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

Appuntamento *find_appuntamento(char *id_cliente, Appuntamento *appuntamenti)
{
    // loop lista
    Appuntamento *app = appuntamenti;
    while (app != NULL)
    {
        if (strcmp(app->id_cliente, id_cliente) == 0)
        {
            // trovato
            return app;
        }

        app = app->next; // prossimo
    }
    return NULL; // non trovato
}

Agente *new_agente(char *id_agente, int n_clienti, Appuntamento *appuntamenti,
                   AgenteNome *id_nomi_agenti, int size_nomi_agenti)
{
    Agente *val = (Agente *)malloc(sizeof(Agente));
    assert(val != NULL);
    // assegna dati
    strcpy(val->id_agente, id_agente);
    val->n_clienti_agente = n_clienti;
    val->appuntamenti = appuntamenti;
    // cerca nome
    AgenteNome *an = find_agente_nome(val->id_agente, id_nomi_agenti, size_nomi_agenti);
    if (an == NULL)
    {
        // non trovato
        strcpy(val->nome_cognome_agente, NOME_AGENTE_DEFAULT);
    }
    else
    {
        // nome presente
        strcpy(val->nome_cognome_agente, an->nome_cognome_agente);
    }
}

// inserimento in testa
void push_agente(Agente *val, Agente **lista)
{
    if (val == NULL)
        return;
    val->next = *lista;
    *lista = val;
}

Agente *find_agente(char *id_agente, Agente *agenti)
{
    // loop lista
    Agente *a = agenti;
    while (a != NULL)
    {
        if (strcmp(a->id_agente, id_agente) == 0)
        {
            // trovato
            return a;
        }

        a = a->next; // prossimo
    }
    return NULL; // non trovato
}

Agente *leggi_appuntamenti(char *filename, AgenteNome *id_nomi_agenti, int size_nomi_agenti)
{
    Agente *agenti = NULL;
    // apri file in lettura
    FILE *file = fopen(filename, "r");
    assert(file != NULL);

    // loop file
    RecordFile record;
    while (fscanf(file, "%s %s %s\n",
                  record.id_cliente, record.id_agente, record.codice_richiesta_prestazione) == 3)
    {
        // cerca agente
        Agente *a = find_agente(record.id_agente, agenti);
        if (a == NULL)
        {
            // nuovo agente
            a = new_agente(record.id_agente, 0, NULL, id_nomi_agenti, size_nomi_agenti);
            push_agente(a, &agenti);
        }

        // cerca se il cliente è nuovo
        Appuntamento *app = find_appuntamento(record.id_cliente, a->appuntamenti);
        if (app == NULL)
        {
            // nuovo cliente
            a->n_clienti_agente++;
        }

        // crea nuovo appuntamento
        app = new_appuntamento(record.id_cliente, record.codice_richiesta_prestazione);
        push_appuntamento(app, &(a->appuntamenti));
    }

    // chiudi file
    fclose(file);
    return agenti;
}

void stampa_appuntamenti(Appuntamento *app)
{
    if (app == NULL)
    {
        // fine lista
        putchar('\n');
        return;
    }

    // stampa dati
    printf("Cliente: %s, Codice Richiesta: %s\n",
           app->id_cliente, app->codice_richiesta_prestazione);

    // ricorsione
    stampa_appuntamenti(app->next);
}

void stampa_agenti(Agente *a)
{
    if (a == NULL)
    {
        // fine lista
        putchar('\n');
        return;
    }

    // stampa dati
    printf("Agente: %s, Nome: %s, Clienti: %d, Appuntamenti:\n",
           a->id_agente, a->nome_cognome_agente, a->n_clienti_agente);
    stampa_appuntamenti(a->appuntamenti);

    // ricorsione
    stampa_agenti(a->next);
}

void free_appuntamenti(Appuntamento *app)
{
    if (app == NULL)
    {
        // fine lista
        return;
    }

    Appuntamento *next = app->next;
    // libera memoria
    free(app);

    // ricorsione
    free_appuntamenti(next);
}

void free_agenti(Agente *a)
{
    if (a == NULL)
    {
        // fine lista
        return;
    }

    Agente *next = a->next;
    // libera memoria
    free_appuntamenti(a->appuntamenti);
    free(a);

    // ricorsione
    free_agenti(next);
}

// inserimento decrescente n_clienti
void push_agente_classifica(Agente *val, Agente **classifica)
{
    if (val == NULL)
        return;

    // cerca posizione in lista
    Agente *current = *classifica;
    Agente *prev = NULL;
    while (current != NULL)
    {
        if (val->n_clienti_agente > current->n_clienti_agente)
        {
            // trovato
            break;
        }

        // prossimo
        prev = current;
        current = current->next;
    }
    // inserisci in mezzo
    val->next = current;
    if (prev != NULL)
    {
        prev->next = val;
    }
    else
    {
        // nuova testa
        *classifica = val;
    }
}

// stampa classifica agenti in base al numero di clienti con più di un appuntamento
void premio_agente_anno(Agente *agenti)
{
    Agente *classifica = NULL;
    // loop agenti
    Agente *a = agenti;
    while (a != NULL)
    {
        // clona agente per classifica (clienti a 0 perchè conterà solo quelli con più richieste)
        Agente *a_classifica = new_agente(a->id_agente, 0, NULL, NULL, 0);
        strcpy(a_classifica->nome_cognome_agente, a->nome_cognome_agente);

        int ripetizioni_cliente = 1;
        // loop appuntamenti
        Appuntamento *app = a->appuntamenti;
        Appuntamento *prev_app = NULL;
        while (app != NULL)
        {
            if (prev_app != NULL) // skip primo
            {
                if (strcmp(prev_app->id_cliente, app->id_cliente) == 0)
                {
                    ripetizioni_cliente++;
                }
                else
                {
                    if (ripetizioni_cliente >= 2)
                    {
                        // cliente con più appuntamenti
                        a_classifica->n_clienti_agente++;
                    }
                    // reset counter
                    ripetizioni_cliente = 1;
                }
            }

            // prossimo
            prev_app = app;
            app = app->next;
        }
        // ripeti controllo per ultimo cliente
        if (ripetizioni_cliente >= 2)
        {
            // cliente con più appuntamenti
            a_classifica->n_clienti_agente++;
        }

        // inserisci agente
        push_agente_classifica(a_classifica, &classifica);

        a = a->next; // prossimo
    }

    // stampa classifica
    int counter = 0;
    a = classifica;
    while (counter < SIZE_CLASSIFICA && a != NULL)
    {
        printf("%d# Agente: %s, Clienti NON Unici: %d\n",
               counter + 1, a->id_agente, a->n_clienti_agente);
        a = a->next; // prossimo
    }
    putchar('\n');
    // libera memoria
    free_agenti(classifica);
}

int main()
{
    AgenteNome id_nomi_agenti[N_AGENTI];
    int size_id_nomi_agenti = 0;
    AgenteNome an = new_agente_nome("Marco", "10021");
    push_agente_nome(an, id_nomi_agenti, &size_id_nomi_agenti);

    Agente *agenti = leggi_appuntamenti(FILENAME, id_nomi_agenti, size_id_nomi_agenti);
    puts("AGENTI:");
    stampa_agenti(agenti);

    puts("CLASSIFICA:");
    premio_agente_anno(agenti);

    free_agenti(agenti);
    return 0;
}