#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

#define FILENAME "appuntamenti.txt"
#define N_MEDICI 200
#define LEN_ID_MEDICO 12
#define LEN_ID_PAZIENTE 12
#define LEN_CODICE_RICHIESTA 25
#define LEN_REPARTO 5
#define N_GIORNI 7
#define N_TURNI_GIORNO 8
// #define MAX_PAZIENTI_TURNO 5
// TEST
#define MAX_PAZIENTI_TURNO 1

#define MAX_PAZIENTI_MEDICO (N_GIORNI * N_TURNI_GIORNO * MAX_PAZIENTI_TURNO)

#define REPARTO_UNDEFINED "ND"         // nome reparto default
#define DIVISORE_ID_MEDICO_REPARTO '-' // carattere id medico che identifica inizio id reparto
#define REALLOC_JUMP 5                 // ogni quanti elementi fare realloc

typedef struct singolo_turno
{
    int id_turno;
    int pazienti_per_turno;

    struct singolo_turno *next;
} singolo_turno;

typedef struct medico
{
    char id_medico[LEN_ID_MEDICO + 1];
    char reparto[LEN_REPARTO + 1];
    singolo_turno *turni[N_GIORNI];
} medico;

typedef struct Appuntamento
{
    char id_paziente[LEN_ID_PAZIENTE + 1];
    char id_medico[LEN_ID_MEDICO + 1];
    char codice_richiesta_prestazione[LEN_CODICE_RICHIESTA + 1];
    int turno_richiesto;
    int giorno_richiesto;
} Appuntamento;

typedef struct EsuberoReparto
{
    char reparto[LEN_REPARTO + 1];
    singolo_turno *turni[N_GIORNI];
} EsuberoReparto;

int push_medico(medico val, medico **vettore, int *size_vettore)
{
    int size = *size_vettore;
    // cerca posizione dove inserire
    int index = -1;
    for (int i = 0; i < size; i++)
    {
        medico current = (*vettore)[i];
        if (strcmp(val.id_medico, current.id_medico) < 0)
        {
            // salva indice
            index = i;
        }
    }
    if (index == -1)
    {
        // inserisci in coda
        index = size;
    }

    // aggiungi spazio se necessario
    if (size % N_MEDICI == 0)
    {
        *vettore = (medico *)realloc(*vettore, sizeof(medico) * (size + N_MEDICI));
        assert(*vettore != NULL);
    }

    // sposta elementi dopo index
    for (int i = size; i > index; i--)
    {
        (*vettore)[i] = (*vettore)[i - 1];
    }
    // inserisci elemento
    (*vettore)[index] = val;
    (*size_vettore)++;

    return index;
}

int len_turni(singolo_turno *turni)
{
    int n_turni = 0;
    // loop lista
    while (turni != NULL)
    {
        // aumenta counter
        n_turni++;

        turni = turni->next;
    }
    return n_turni;
}

void push_turno(singolo_turno *val, singolo_turno **lista)
{
    // inserimento in testa
    val->next = *lista;
    *lista = val;
}

int get_reparto_index(char *id_medico)
{
    int reparto = -1;
    int start = -1;
    for (int i = 0; i < LEN_ID_MEDICO + 1; i++)
    {
        // prossima lettera
        char c = id_medico[i];
        if (c == DIVISORE_ID_MEDICO_REPARTO)
        {
            // salva partenza
            start = i + 1;
            break;
        }
    }
    if (start != -1)
    {
        // reparto presente
        reparto = atoi(&(id_medico[start]));
    }

    return reparto;
}

medico *find_medico(char *id_medico, medico *medici, int size_medici)
{
    // binary search
    if (size_medici < 1)
    {
        // vettore vuoto
        return NULL;
    }
    int mid = size_medici / 2;
    int compare_val = strcmp(id_medico, medici[mid].id_medico);
    if (compare_val == 0)
    {
        // medico trovato
        return &(medici[mid]);
    }
    if (size_medici == 1)
    {
        // non più divisibile
        return NULL;
    }
    if (compare_val > 0)
    {
        // cerca a destra
        int start = mid + 1;
        return find_medico(id_medico, &(medici[start]), size_medici - start);
    }
    if (compare_val < 0)
    {
        // cerca a sinistra
        return find_medico(id_medico, &(medici[0]), mid);
    }
}

void push_reparto(char *reparto, int index, char ***repartiMedici, int *size_repartiMedici)
{
    int size = *size_repartiMedici;
    // aggiungi spazio necessario
    if (index >= size)
    {
        // ERRORE aumentata size solo di 1
        (*size_repartiMedici) = index + 1;
        // ERRORE sizeof di len stringa
        *repartiMedici = (char **)realloc(*repartiMedici,
                                          sizeof(char *) * (*size_repartiMedici));
        assert(*repartiMedici != NULL);

        // metti valori default a nuovi
        for (int i = size; i < index + 1; i++)
        {
            (*repartiMedici)[i] = REPARTO_UNDEFINED;
        }
    }
    // aggiungi reparto
    (*repartiMedici)[index] = reparto;
}

void set_reparto(medico *m, int index, char ***repartiMedici, int *size_repartiMedici)
{
    if (index >= *size_repartiMedici)
    {
        // reparto inesistente
        strcpy(m->reparto, REPARTO_UNDEFINED);
        push_reparto(m->reparto, index, repartiMedici, size_repartiMedici);
    }
    // reparto presente
    strcpy(m->reparto, (*repartiMedici)[index]);
}

medico crea_medico(char *id_medico, char ***repartiMedici, int *size_repartiMedici)
{
    medico m;
    // assegna dati
    strcpy(m.id_medico, id_medico);
    // calcola reparto
    int reparto_index = get_reparto_index(m.id_medico);
    set_reparto(&m, reparto_index, repartiMedici, size_repartiMedici);
    // azzera turni
    for (int i = 0; i < N_GIORNI; i++)
    {
        m.turni[i] = NULL;
    }
    return m;
}

singolo_turno *find_turno(int id, singolo_turno *turni)
{
    singolo_turno *t = turni;
    // loop lista
    while (t != NULL)
    {
        if (t->id_turno == id)
        {
            // trovato
            return t;
        }

        t = t->next; // prossimo
    }
    return NULL; // non trovato
}

singolo_turno *crea_turno(int id, int pazienti_per_turno)
{
    singolo_turno *t = (singolo_turno *)malloc(sizeof(singolo_turno));
    assert(t != NULL);
    // assegna dati
    t->id_turno = id;
    t->pazienti_per_turno = pazienti_per_turno;
    t->next = NULL;

    return t;
}

EsuberoReparto *find_esubero(char *reparto, EsuberoReparto *esuberi, int size_esuberi)
{
    for (int i = 0; i < size_esuberi; i++)
    {
        if (strcmp(reparto, esuberi[i].reparto) == 0)
        {
            // reparto trovato
            return &(esuberi[i]);
        }
    }
    return NULL; // non torvato
}

EsuberoReparto crea_esubero(char *reparto)
{
    EsuberoReparto er;
    strcpy(er.reparto, reparto);
    // azzera turni
    for (int i = 0; i < N_GIORNI; i++)
    {
        er.turni[i] = NULL;
    }

    return er;
}

int push_esubero(EsuberoReparto val, EsuberoReparto **vettore, int *size_vettore)
{
    int size = *size_vettore;
    // aggiungi spazio
    if (size % REALLOC_JUMP == 0)
    {
        *vettore = (EsuberoReparto *)realloc(*vettore, sizeof(EsuberoReparto) * (size + REALLOC_JUMP));
        assert(*vettore != NULL);
    }
    // inserisci elemento in coda
    (*vettore)[size] = val;
    (*size_vettore)++;

    return size;
}

void leggi_appuntamenti(char *filename, char ***repartiMedici, int *size_repartiMedici,
                        medico **out_medici, int *out_size_medici,
                        EsuberoReparto **out_esuberi, int *out_size_esuberi)
{
    medico *medici = NULL;
    int size_medici = 0;
    EsuberoReparto *esuberi = NULL;
    int size_esuberi = 0;
    // apri file in lettura
    FILE *file = fopen(filename, "r");
    assert(file != NULL);

    // loop file
    while (!feof(file))
    {
        // leggi primo dato
        char id_paziente[LEN_ID_PAZIENTE + 1] = {0};
        fscanf(file, "%s", id_paziente);
        if (strlen(id_paziente) == 0)
        {
            // riga vuota finale
            break;
        }

        // nuovo appuntamento
        Appuntamento a;
        strcpy(a.id_paziente, id_paziente);
        // leggi dati restanti
        fscanf(file, "%s %s %d %d\n",
               a.id_medico, a.codice_richiesta_prestazione, &(a.turno_richiesto), &(a.giorno_richiesto));

        // prendi medico richiesto
        medico *m = find_medico(a.id_medico, medici, size_medici);
        if (m == NULL)
        {
            // nuovo medico
            medico nuovo_medico = crea_medico(a.id_medico, repartiMedici, size_repartiMedici);
            int index = push_medico(nuovo_medico, &medici, &size_medici);
            // seleziona nuovo medico
            m = &(medici[index]);
        }

        // prendi turno richiesto
        // ERRORE giorno sfora di 1
        int giorno_index = a.giorno_richiesto - 1;
        singolo_turno *t = find_turno(a.turno_richiesto, m->turni[giorno_index]);
        if (t == NULL)
        {
            // nuovo turno
            t = crea_turno(a.turno_richiesto, 0);
            push_turno(t, &(m->turni[giorno_index]));
        }
        // aumenta pazienti
        if (t->pazienti_per_turno < MAX_PAZIENTI_TURNO)
        {
            t->pazienti_per_turno++;
        }
        else
        {
            // esubero
            EsuberoReparto *er = find_esubero(m->reparto, esuberi, size_esuberi);
            if (er == NULL)
            {
                // nuovo reparto
                EsuberoReparto nuovo_esubero = crea_esubero(m->reparto);
                int index = push_esubero(nuovo_esubero, &esuberi, &size_esuberi);
                // seleziona esubero
                er = &(esuberi[index]);
            }

            // prendi turno esubero
            singolo_turno *t_esubero = find_turno(a.turno_richiesto, er->turni[giorno_index]);
            if (t_esubero == NULL)
            {
                // nuovo turno
                t_esubero = crea_turno(a.turno_richiesto, 0);
                push_turno(t_esubero, &(er->turni[giorno_index]));
            }
            // aumenta pazienti
            t_esubero->pazienti_per_turno++;
        }
    }

    // chiudi file
    fclose(file);
    // parametri in output
    *out_medici = medici;
    *out_size_medici = size_medici;
    *out_esuberi = esuberi;
    *out_size_esuberi = size_esuberi;
}

// struct di supporto per occupazione_reparti
typedef struct RepartoOccupazione
{
    char reparto[LEN_REPARTO + 1];
    float percentuale_occupazione;
    int pazienti;
    int totale_pazienti;

    struct RepartoOccupazione *next;
} RepartoOccupazione;

RepartoOccupazione *find_reparto_occ(char *reparto, RepartoOccupazione *lista)
{
    RepartoOccupazione *ro = lista;
    // loop lista
    while (ro != NULL)
    {
        if (strcmp(reparto, ro->reparto) == 0)
        {
            // trovato
            return ro;
        }

        ro = ro->next; // prossimo
    }

    return NULL; // non trovato
}

RepartoOccupazione *crea_reparto_occ(char *reparto)
{
    RepartoOccupazione *ro = (RepartoOccupazione *)malloc(sizeof(RepartoOccupazione));
    assert(ro != NULL);
    // assegna dati
    strcpy(ro->reparto, reparto);
    ro->percentuale_occupazione = ro->pazienti = ro->totale_pazienti = 0;

    return ro;
}

void push_reparto_occ(RepartoOccupazione *val, RepartoOccupazione **lista)
{
    val->next = *lista;
    *lista = val;
}

RepartoOccupazione *occupazione_reparti(medico *medici, int size_medici)
{
    RepartoOccupazione *occupazioni = NULL;

    for (int i = 0; i < size_medici; i++)
    {
        medico m = medici[i];
        // cerca reparto
        RepartoOccupazione *ro = find_reparto_occ(m.reparto, occupazioni);
        if (ro == NULL)
        {
            // nuovo reparto
            ro = crea_reparto_occ(m.reparto);
            push_reparto_occ(ro, &occupazioni);
        }
        // aggiorna valori
        ro->totale_pazienti += MAX_PAZIENTI_MEDICO;
        // loop turni
        for (int j = 0; j < N_GIORNI; j++)
        {
            singolo_turno *t = m.turni[j];
            while (t != NULL)
            {
                ro->pazienti += t->pazienti_per_turno;

                t = t->next; // prossimo
            }
        }
    }

    // calcola percentuali
    RepartoOccupazione *ro = occupazioni;
    while (ro != NULL)
    {
        ro->percentuale_occupazione = ((float)(ro->pazienti) / (float)(ro->totale_pazienti)) * 100.0;

        ro = ro->next; // prossimo
    }

    return occupazioni;
}

void stampa_esuberi(EsuberoReparto *esuberi, int i, int size_esuberi, int *pazienti_giorno)
{
    if (i == size_esuberi)
    {
        // fine vettore
        puts("Esuberi per giorno:");
        for (int i = 0; i < N_GIORNI; i++)
        {
            printf("Giorno %d: %d pazienti\n", i + 1, pazienti_giorno[i]);
        }
        putchar('\n');
        return;
    }

    // prendi esubero
    EsuberoReparto er = esuberi[i];
    // loop turni
    for (int j = 0; j < N_GIORNI; j++)
    {
        singolo_turno *t = er.turni[j];
        while (t != NULL)
        {
            // aggiorna totale giorno
            pazienti_giorno[j] += t->pazienti_per_turno;

            t = t->next; // prossimo
        }
    }

    // ricorsione
    stampa_esuberi(esuberi, i + 1, size_esuberi, pazienti_giorno);
}

void free_reparto_occ(RepartoOccupazione *ro)
{
    if (ro == NULL)
    {
        // fine lista
        return;
    }

    // salva prossimo e libera memoria
    RepartoOccupazione *next = ro->next;
    free(ro);

    free_reparto_occ(next); // ricorsione
}

// TEST
void stampa_settimana(singolo_turno **turni)
{
    for (int i = 0; i < N_GIORNI; i++)
    {
        singolo_turno *t = turni[i];
        if (t != NULL)
        {
            printf("Giorno: %d\n", i + 1);
            while (t != NULL)
            {
                printf("\tTurno: %d, Pazienti: %d\n", t->id_turno, t->pazienti_per_turno);

                t = t->next;
            }
        }
    }
    putchar('\n');
}

// TEST
void stampa_medici(medico *medici, int size)
{
    for (int i = 0; i < size; i++)
    {
        medico m = medici[i];
        printf("Medico: %s, Reparto: %s, Turni:\n",
               m.id_medico, m.reparto);
        stampa_settimana(m.turni);
    }
}

// TEST
void stampa_esuberi_test(EsuberoReparto *esuberi, int size)
{
    for (int i = 0; i < size; i++)
    {
        EsuberoReparto er = esuberi[i];
        printf("Reparto: %s, Turni:\n",
               er.reparto);
        stampa_settimana(er.turni);
    }
}

// TEST
void stampa_occupazioni(RepartoOccupazione *ro)
{
    while (ro != NULL)
    {
        printf("Reparto: %s, Occupazione: %.2f%% (%d / %d)\n",
               ro->reparto, ro->percentuale_occupazione, ro->pazienti, ro->totale_pazienti);

        ro = ro->next;
    }
    putchar('\n');
}

int main()
{
    // prepara vettore repartiMedici
    char **repartiMedici = NULL;
    int size_repartiMedici = 0;
    // valore test
    push_reparto("OCT", 21, &repartiMedici, &size_repartiMedici);

    // riempi medici ed esuberi
    medico *medici = NULL;
    int size_medici = 0;
    EsuberoReparto *esuberi = NULL;
    int size_esuberi = 0;
    leggi_appuntamenti(FILENAME, &repartiMedici, &size_repartiMedici,
                       &medici, &size_medici, &esuberi, &size_esuberi);

    // TEST
    puts("Medici:");
    stampa_medici(medici, size_medici);
    puts("Esuberi:");
    stampa_esuberi_test(esuberi, size_esuberi);

    // stampa esuberi
    int pazienti_giorno[N_GIORNI] = {0}; // necessario per mantenere i valori nella funzione ricorsiva
    stampa_esuberi(esuberi, 0, size_esuberi, pazienti_giorno);
    // calcola occupazione
    RepartoOccupazione *occupazioni = occupazione_reparti(medici, size_medici);

    // TEST
    puts("Occupazione");
    stampa_occupazioni(occupazioni);

    // libera memoria
    free(medici);
    free(esuberi);
    free(repartiMedici);
    free_reparto_occ(occupazioni);
    return 0;
}