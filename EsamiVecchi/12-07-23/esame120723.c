#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

#define FILENAME "appuntamenti.txt"
#define LEN_ID_PAZIENTE 15
#define LEN_ID_MEDICO 20
#define LEN_NOME_CONGOME_PAZIENTE 100
#define PERCENTUALE_SCONTO 15
#define TARIFFA_STANDARD_ORARIA 100
#define ORE_NECESSARIE_PER_SCONTO 10

// da non modificare
#define MOLTIPLICATORE_SCONTO (1 - (PERCENTUALE_SCONTO / 100))

// TEST
#define ID_STAMPA_AGENDA "Cliente1"

// len stringa = caratteri utili + terminatore

typedef struct Data
{
    int giorno;
    int mese;
    int anno;
} Data;

typedef struct Appuntamento
{
    Data data;
    int ora_inizio;
    int ora_fine;

    struct Appuntamento *next;
} Appuntamento;

typedef struct RecordFile
{
    char id_paziente[LEN_ID_PAZIENTE + 1];
    char id_medico[LEN_ID_PAZIENTE + 1];
    char nome_e_cognome[LEN_NOME_CONGOME_PAZIENTE + 1];

    Appuntamento *appuntamenti;
} RecordFile;

typedef struct Tariffa
{
    char id_medico[LEN_ID_PAZIENTE + 1];
    float costo_orario;
} Tariffa;

typedef struct Paziente
{
    char id_paziente[LEN_ID_PAZIENTE + 1];
    char id_medico[LEN_ID_PAZIENTE + 1];
    float preventivo;

    Appuntamento *appuntamenti;
    struct Paziente *next;
} Paziente;

int insert_tariffa(Tariffa val, Tariffa **vettore, int *size)
{
    int prev_size = *size;
    // aggiungi cella
    *vettore = (Tariffa *)realloc(*vettore, sizeof(Tariffa) * (prev_size + 1));
    assert(*vettore != NULL);
    (*size)++;

    int index = -1;
    // trova indice cella dove inserire nuovo elemento
    for (int i = 0; i < prev_size; i++)
    {
        Tariffa current = (*vettore)[i];
        if (strcmp(val.id_medico, current.id_medico) < 0)
        {
            // salva indice
            index = i;
            break;
        }
    }
    if (index == -1)
    {
        // inserisci in coda
        index = prev_size;
    }
    // sposta successivi a index di 1 a destra
    for (int i = prev_size; i > index; i--)
    {
        (*vettore)[i] = (*vettore)[i - 1];
    }
    // inserisci in posizione corretta
    (*vettore)[index] = val;
    // ritorna posizione di inserimento
    return index;
}

void insert_appuntamento(Appuntamento *val, Appuntamento **lista)
{
    if (val == NULL || lista == NULL)
        return;

    Appuntamento *prev = NULL;
    Appuntamento *current = *lista;
    // scorri fino in fondo
    while (current != NULL)
    {
        // passa al prossimo
        prev = current;
        current = current->next;
    }

    // inserisci in posizione
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

bool stesso_paziente(Paziente *p1, Paziente *p2)
{
    // stesso id e stesso medico
    return (strcmp(p1->id_paziente, p2->id_paziente) == 0 &&
            strcmp(p1->id_medico, p2->id_medico) == 0);
}

Paziente paziente_temporaneo(char *id_paziente, char *id_medico)
{
    // crea paziente fittizio
    Paziente tmp;
    strcpy(tmp.id_paziente, id_paziente);
    strcpy(tmp.id_medico, id_medico);
    return tmp;
}

Paziente *get_paziente(char *id_paziente, char *id_medico, Paziente *pazienti)
{
    // scorri pazienti
    while (pazienti != NULL)
    {
        Paziente tmp = paziente_temporaneo(id_paziente, id_medico);
        if (stesso_paziente(&tmp, pazienti))
        {
            // paziente trovato
            return pazienti;
        }

        pazienti = pazienti->next; // passa al prossimo
    }
    return NULL; // paziente non trovato
}

void insert_paziente(Paziente *val, Paziente **lista)
{
    if (val == NULL)
        return;

    Paziente *prev = NULL;
    Paziente *current = *lista;
    // scorri lista
    while (current != NULL)
    {
        if (strcmp(val->id_paziente, current->id_paziente) < 0)
        {
            // trovata posizione di inserimento
            break;
        }

        // passa al prossimo
        prev = current;
        current = current->next;
    }

    // inserisci in posizione
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

Paziente *leggi_appuntamenti(char *filename)
{
    Paziente *pazienti = NULL;
    // apri file in lettura
    FILE *file = fopen(filename, "r");
    assert(file != NULL);

    // loop file
    while (!feof(file))
    {
        /*
        Struttura riga:
        ID_PAZIENTE ID_MEDICO NOME_COGNOME_PAZIENTE GiornoA1/MeseA1/AnnoA1 ora_inizioA1 ora_fineA1 GiornoA2/MeseA2/AnnoA2 ora_inizioA2 ora_fineA2 ...
        Esempio riga
        Cliente0 Medico0 Mario_Rossi 01/01/2001 9 10 02/03/2001 15 17 ...
        */
        RecordFile record;
        // leggi dati da file
        // ERRORE id_paziente e id_medico invertiti
        fscanf(file, "%s %s %s", record.id_paziente, record.id_medico, record.nome_e_cognome);
        record.appuntamenti = NULL;

        // leggi appuntamenti
        char c = 0;
        while ((c = fgetc(file)) == ' ')
        {
            // nuovo appuntamento
            Appuntamento *a = (Appuntamento *)malloc(sizeof(Appuntamento));
            assert(a != NULL);
            fscanf(file, "%d/%d/%d %d %d",
                   &(a->data.giorno), &(a->data.mese), &(a->data.anno), &(a->ora_inizio), &(a->ora_fine));
            // inserisci in coda
            insert_appuntamento(a, &(record.appuntamenti));
        }

        // cerca paziente corrispondente
        Paziente *p = get_paziente(record.id_paziente, record.id_medico, pazienti);
        if (p == NULL)
        {
            // nuovo paziente
            p = (Paziente *)malloc(sizeof(Paziente));
            assert(p != NULL);

            // assegna dati
            p->preventivo = 0.0;
            strcpy(p->id_paziente, record.id_paziente);
            strcpy(p->id_medico, record.id_medico);
            p->appuntamenti = NULL;
            // inserisci in lista
            insert_paziente(p, &(pazienti));
        }

        // accoda appuntamenti letti
        Appuntamento *current = record.appuntamenti;
        while (current != NULL)
        {
            Appuntamento *next = current->next; // salva prossimo

            // crea copia nodo
            Appuntamento *copia = (Appuntamento *)malloc(sizeof(Appuntamento));
            assert(copia != NULL);
            *copia = *current;
            // inserisci in testa
            copia->next = p->appuntamenti;
            p->appuntamenti = copia;
            // elimina corrente
            free(current);

            current = next; // passa al prossimo
        }
    }

    fclose(file); // chiudi file
    return pazienti;
}

void stampa_agenda_paziente(char *id_paziente, Paziente *pazienti)
{
    printf("Appuntamenti paziente %s:\n", id_paziente);
    // scorri pazienti
    while (pazienti != NULL)
    {
        if (strcmp(id_paziente, pazienti->id_paziente) == 0)
        {
            // paziente interessato
            printf("Medico %s:\n", pazienti->id_medico);
            // stampa dati appuntamenti
            Appuntamento *a = pazienti->appuntamenti;
            // scorri appuntamenti con quel medico
            while (a != NULL)
            {
                // stampa dati appuntamento
                printf("\tData: %d/%d/%d, Ora inizio: %d, Ora fine: %d\n",
                       a->data.giorno, a->data.mese, a->data.anno, a->ora_inizio, a->ora_fine);
                a = a->next; // passa al prossimo
            }
        }

        pazienti = pazienti->next; // passa al prossimo
    }
    putchar('\n');
}

Tariffa *get_tariffa(char *id_medico, Tariffa *tariffe, int size_tariffe)
{
    // binary search
    if (size_tariffe < 1)
        return NULL;

    int mid = size_tariffe / 2;
    // salva valore a metà
    char mid_val[LEN_ID_MEDICO + 1];
    strcpy(mid_val, tariffe[mid].id_medico);
    // salva comparazione
    int compare_result = strcmp(id_medico, mid_val);
    if (compare_result == 0)
    {
        // medico trovato
        return &(tariffe[mid]);
    }
    else if (size_tariffe == 1)
    {
        // non più divisibile
        return NULL; // non trovato
    }
    else if (compare_result > 0)
    {
        // cerca in metà destra
        int start = mid + 1;
        return get_tariffa(id_medico, &(tariffe[start]), size_tariffe - start);
    }
    else if (compare_result < 0)
    {
        // cerca in metà sinistra
        return get_tariffa(id_medico, &(tariffe[0]), mid);
    }
}

void calcola_preventivi(Paziente *pazienti, Tariffa **tariffe, int *size_tariffe)
{
    while (pazienti != NULL)
    {
        // prendi tariffa medico
        Tariffa *tariffa = get_tariffa(pazienti->id_medico, *tariffe, *size_tariffe);
        if (tariffa == NULL)
        {
            // crea tariffa medico default
            Tariffa t;
            strcpy(t.id_medico, pazienti->id_medico);
            t.costo_orario = TARIFFA_STANDARD_ORARIA;
            // inserisci e seleziona
            int index = insert_tariffa(t, tariffe, size_tariffe);
            tariffa = &((*tariffe)[index]);
        }

        // aggiungi costo ogni appuntamento
        int somma_ore = 0;
        Appuntamento *a = pazienti->appuntamenti;
        // scorri appuntamenti con quel medico
        while (a != NULL)
        {
            // calcola durata appuntamento e aggiungila al totale
            int durata = a->ora_fine - a->ora_inizio;
            somma_ore += durata;
            // aggiungi al preventivo
            pazienti->preventivo += tariffa->costo_orario * ((float)durata);

            a = a->next; // passa al prossimo
        }
        if (somma_ore >= ORE_NECESSARIE_PER_SCONTO)
        {
            // applica sconto al totale
            pazienti->preventivo *= MOLTIPLICATORE_SCONTO;
        }

        pazienti = pazienti->next; // passa al prossimo
    }
}

void free_appuntamenti(Appuntamento *a)
{
    if (a == NULL)
        return;
    // ERRORE copia incolla free pazienti
    Appuntamento *next = a->next;
    free(a);

    free_appuntamenti(next);
}

void free_pazienti(Paziente *p)
{
    if (p == NULL)
        return;

    Paziente *next = p->next;
    free_appuntamenti(p->appuntamenti);
    free(p);

    free_pazienti(next);
}

// TEST
void stampa_preventivi(Paziente *p, Tariffa *tariffe, int size_tariffe)
{
    puts("Preventivi pazienti:");
    while (p != NULL)
    {
        printf("Paziente: %s, Medico %s:\n", p->id_paziente, p->id_medico);
        // stampa dati appuntamenti
        Appuntamento *a = p->appuntamenti;
        // scorri appuntamenti con quel medico
        while (a != NULL)
        {
            // stampa dati appuntamento
            printf("\tData: %d/%d/%d, Ora inizio: %d, Ora fine: %d\n",
                   a->data.giorno, a->data.mese, a->data.anno, a->ora_inizio, a->ora_fine);
            a = a->next; // passa al prossimo
        }

        Tariffa *t = get_tariffa(p->id_medico, tariffe, size_tariffe);
        assert(t != NULL);
        printf("Tariffa: %.2f, TOTALE: %.2f\n\n", t->costo_orario, p->preventivo);
        p = p->next;
    }
}

int main()
{
    Paziente *pazienti = leggi_appuntamenti(FILENAME);
    stampa_agenda_paziente(ID_STAMPA_AGENDA, pazienti);

    Tariffa *tariffe = NULL;
    int size_tariffe = 0;
    // TEST
    insert_tariffa(((Tariffa){"Medico0", 150.5}), &tariffe, &size_tariffe);

    calcola_preventivi(pazienti, &tariffe, &size_tariffe);

    // TEST
    stampa_preventivi(pazienti, tariffe, size_tariffe);

    free_pazienti(pazienti);
    free(tariffe);
    return 0;
}