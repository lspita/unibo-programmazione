// Ludovico Maria Spitaleri
// Matricola 0001114169

#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define FILENAME "appuntamenti.txt"
#define N_REPARTI 5
#define LEN_ID_REPARTO 5
#define N_TURNI 7
#define MAX_TURNI_GIORNO 8
#define ORE_TURNO 3
#define MEDICI_TURNO 2
#define LEN_ID_MEDICO 12
#define LEN_ID_PAZIENTE 12
#define LEN_CODICE_RICHIESTA 25
#define N_ESUBERI 100
#define MIN_PAZIENTI_TURNO 0
#define MAX_PAZIENTI_TURNO 2

// len stringa = caratteri utili + terminatore

typedef struct singolo_turno
{
    int id_turno;
    int pazienti_per_turno;
    int pazienti_correnti_1; // numero di pazienti medico1
    int pazienti_correnti_2; // numero di pazienti medico2
    char id_medico1[LEN_ID_MEDICO + 1];
    char id_medico2[LEN_ID_MEDICO + 1];
} singolo_turno;

typedef struct reparto
{
    char id_reparto[LEN_ID_REPARTO + 1];
    singolo_turno turni[N_TURNI][MAX_TURNI_GIORNO];
} reparto;

typedef struct appuntamento
{
    char id_paziente[LEN_ID_PAZIENTE + 1];
    char id_medico[LEN_ID_MEDICO + 1];
    char codice_richiesta_prestazione[LEN_CODICE_RICHIESTA + 1];
    int turno_richiesto;
    int giorno_richiesto;
} appuntamento;

typedef struct reparto_medico
{
    char codice_richiesta_prestazione[LEN_CODICE_RICHIESTA + 1];
    char id_reparto[LEN_ID_REPARTO + 1];
} reparto_medico;

int randint(int min, int max)
{
    return (int)(rand() % (max - min + 1)) + min;
}

#define BOLD "\033[1m"
#define RESET "\033[0m"

void puts_bold(char *msg)
{
    printf("%s%s%s\n", BOLD, msg, RESET);
}

int insert_reparto(reparto val, reparto **vettore, int *size_vettore)
{
    // aggiungi cella
    *vettore = (reparto *)realloc(*vettore, sizeof(reparto) * ((*size_vettore) + 1));
    assert(vettore != NULL);

    int index = -1;
    for (int i = 0; i < (*size_vettore); i++)
    {
        if (strcmp(val.id_reparto, (*vettore)[i].id_reparto) < 0)
        {
            index = i; // salva indice dove verrà inserito
            break;
        }
    }
    if (index == -1)
    {
        // maggiore di tutti
        index = (*size_vettore);
    }
    // ERRORE for dal fondo
    for (int i = (*size_vettore); i > index; i--)
    {
        // sposta tutti di uno
        (*vettore)[i] = (*vettore)[i - 1];
    }
    // inserisci
    (*vettore)[index] = val;
    (*size_vettore)++;

    return index; // ritorna posizione in cui è stato inserito
}

char *get_id_reparto(char *codice_richiesta_prestazione, reparto_medico *repartiMedici, int size_repartiMedici)
{
    for (int i = 0; i < size_repartiMedici; i++)
    {
        if (strcmp(codice_richiesta_prestazione, repartiMedici[i].codice_richiesta_prestazione) == 0)
        {
            // reparto trovato
            return repartiMedici[i].id_reparto;
        }
    }
    return NULL; // reparto non trovato
}

reparto *get_reparto(char *id_reparto, reparto **reparti, int *size_reparti)
{
    reparto *selected = NULL;
    for (int i = 0; i < *size_reparti; i++)
    {
        reparto *r = &((*reparti)[i]);
        if (strcmp(r->id_reparto, id_reparto) == 0)
        {
            selected = r; // prendi reparto interessato
            break;
        }
    }
    if (selected == NULL)
    {
        // nuovo reparto
        reparto r;
        strcpy(r.id_reparto, id_reparto);
        for (int i = 0; i < N_TURNI; i++)
        {
            for (int j = 0; j < MAX_TURNI_GIORNO; j++)
            {
                // init turni
                singolo_turno t;
                t.id_turno = j + 1;
                strcpy(t.id_medico1, "");
                strcpy(t.id_medico2, "");
                t.pazienti_per_turno = randint(MIN_PAZIENTI_TURNO, MAX_PAZIENTI_TURNO);
                t.pazienti_correnti_1 = 0;
                t.pazienti_correnti_2 = 0;
                r.turni[i][j] = t;
            }
        }

        int index = insert_reparto(r, reparti, size_reparti);
        selected = &((*reparti)[index]); // seleziona nuovo
    }

    return selected; // ritorna selezionato
}

void leggi_appuntamenti(char *filename,
                        reparto_medico *repartiMedici, int size_repartiMedici,
                        reparto **out_reparti, int *out_size_reparti,
                        reparto **out_esuberi, int *out_size_esuberi)
{
    FILE *stream = fopen(filename, "r"); // leggi file
    assert(stream != NULL);
    // inizializza reparti
    int size_reparti = 0;
    reparto *reparti = NULL;
    int size_esuberi = 0;
    reparto *esuberi = NULL;

    while (!feof(stream))
    {
        // loop file
        char id_paziente[LEN_ID_PAZIENTE + 1] = {0};
        fscanf(stream, "%s", id_paziente);
        if (strlen(id_paziente) == 0)
        {
            // riga vuota / fine file letto erroneamente
            break;
        }
        // leggi appuntamento
        appuntamento a;
        strcpy(a.id_paziente, id_paziente);
        fscanf(stream, "%s %s %d %d\n",
               a.id_medico, a.codice_richiesta_prestazione,
               &(a.turno_richiesto), &(a.giorno_richiesto));

        char *id_reparto = get_id_reparto(a.codice_richiesta_prestazione, repartiMedici, size_repartiMedici);
        if (id_reparto != NULL)
        {
            // reparto esistente
            reparto *selected = get_reparto(id_reparto, &reparti, &size_reparti);
            // prendi turno interessato
            singolo_turno *turno = &(selected->turni[a.giorno_richiesto - 1][a.turno_richiesto - 1]);
            // prendi medico 1 o 2
            int medico = 0;
            if (strcmp(turno->id_medico1, a.id_medico) == 0)
            {
                medico = 1;
            }
            else if (strcmp(turno->id_medico2, a.id_medico) == 0)
            {
                medico = 2;
            }
            else if (medico == 0)
            {
                // nuovo medico
                if (strcmp(turno->id_medico1, "") == 0)
                {
                    strcpy(turno->id_medico1, a.id_medico);
                    medico = 1;
                }
                else if (strcmp(turno->id_medico2, "") == 0)
                {
                    strcpy(turno->id_medico2, a.id_medico);
                    medico = 2;
                }
            }
            int *pazienti_medico = NULL;
            if (medico == 1)
            {
                pazienti_medico = &(turno->pazienti_correnti_1);
            }
            else if (medico == 2)
            {
                pazienti_medico = &(turno->pazienti_correnti_2);
            }

            // aggiorna numero pazienti
            if (turno->pazienti_correnti_1 + turno->pazienti_correnti_2 < turno->pazienti_per_turno)
            {
                (*pazienti_medico)++;
            }
            else
            {
                // esubero

                // riprendi reparto e turno in esuberi
                selected = get_reparto(id_reparto, &esuberi, &size_esuberi);
                singolo_turno *t_esubero = &(selected->turni[a.giorno_richiesto - 1][a.turno_richiesto - 1]);
                // aggiorna dati turno
                strcpy(t_esubero->id_medico1, turno->id_medico1);
                strcpy(t_esubero->id_medico2, turno->id_medico2);
                t_esubero->pazienti_per_turno = turno->pazienti_per_turno;
                // aggiungi esubero

                pazienti_medico = NULL;
                if (medico == 1)
                {
                    pazienti_medico = &(t_esubero->pazienti_correnti_1);
                }
                else if (medico == 2)
                {
                    pazienti_medico = &(t_esubero->pazienti_correnti_2);
                }
                (*pazienti_medico)++;
            }
        }
    }

    fclose(stream); // chiudi file
    // ritorna valori con parametri in output
    *out_reparti = reparti;
    *out_size_reparti = size_reparti;
    *out_esuberi = esuberi;
    *out_size_esuberi = size_esuberi;
}

void occupazione_reparti(reparto *reparti, int size_reparti)
{
    for (int i = 0; i < size_reparti; i++)
    {
        // per ogni reparto
        reparto r = reparti[i];
        float pazienti_totali = 0.0, pazienti_max = 0.0;
        // conta somma pazienti correnti e totali
        for (int j = 0; j < N_TURNI; j++)
        {
            for (int k = 0; k < MAX_TURNI_GIORNO; k++)
            {
                singolo_turno t = r.turni[j][k];
                pazienti_max += t.pazienti_per_turno;
                pazienti_totali += t.pazienti_correnti_1;
                // ERRORE non aggiunti correnti_2
                pazienti_totali += t.pazienti_correnti_2;
            }
        }
        // calcola e stampa percentuale
        float percentuale = (pazienti_totali / pazienti_max) * 100;
        printf("Occupazione reparto %s: %.2f%%\n", r.id_reparto, percentuale);
    }
    putchar('\n');
}

typedef struct medico_esuberi
{
    char id_medico[LEN_ID_MEDICO + 1];
    int esuberi;
} medico_esuberi;

medico_esuberi *get_medico(char *id_medico, medico_esuberi *medici, int size_medici)
{
    for (int i = 0; i < size_medici; i++)
    {
        if (strcmp(id_medico, medici[i].id_medico) == 0)
        {
            // trovato
            return &(medici[i]);
        }
    }
    return NULL;
}

void insert_medico(medico_esuberi m, medico_esuberi **vettore, int *size_vettore)
{
    // aggiungi cella
    // ERRORE: avevo lasciato struct reparto dal copia_incolla
    *vettore = (medico_esuberi *)realloc(*vettore, sizeof(medico_esuberi) * ((*size_vettore) + 1));
    assert(vettore != NULL);
    // aggiungi medico in coda
    (*vettore)[*size_vettore] = m;
    (*size_vettore)++;
}

void medici_esuberi(reparto *esuberi, int size_esuberi)
{
    // ERRORE controllo no esuberi
    if (size_esuberi == 0)
    {
        printf("Nessun medico con esuberi\n\n");
        return;
    }
    medico_esuberi *medici = NULL;
    int size_medici = 0;
    for (int i = 0; i < size_esuberi; i++)
    {
        reparto e = esuberi[i];
        // per ogni turno
        for (int j = 0; j < N_TURNI; j++)
        {
            for (int k = 0; k < MAX_TURNI_GIORNO; k++)
            {
                singolo_turno t = e.turni[j][k];
                // prendi medico1
                medico_esuberi *m;
                // se c'è il primo medico
                if (strlen(t.id_medico1) > 0)
                {
                    m = get_medico(t.id_medico1, medici, size_medici);
                    if (m == NULL)
                    {
                        // nuovo medico
                        medico_esuberi val;
                        strcpy(val.id_medico, t.id_medico1);
                        val.esuberi = 0;
                        insert_medico(val, &medici, &size_medici);
                        // selezionalo
                        m = &(medici[size_medici - 1]);
                    }

                    m->esuberi += t.pazienti_correnti_1;
                }
                // se c'è il secondo medico
                if (strlen(t.id_medico2) > 0)
                {
                    m = get_medico(t.id_medico2, medici, size_medici);
                    if (m == NULL)
                    {
                        // nuovo medico
                        medico_esuberi val;
                        strcpy(val.id_medico, t.id_medico2);
                        val.esuberi = 0;
                        insert_medico(val, &medici, &size_medici);
                        // selezionalo
                        m = &(medici[size_medici - 1]);
                    }

                    m->esuberi += t.pazienti_correnti_2;
                }
            }
        }
    }
    int index_max_medico = -1;
    int max_esuberi = -1;
    // trova vincitore
    for (int i = 0; i < size_medici; i++)
    {
        medico_esuberi m = medici[i];
        printf("Medico %s, Esuberi: %d\n", m.id_medico, m.esuberi);
        if (m.esuberi > max_esuberi)
        {
            // nuovo vincitore
            max_esuberi = m.esuberi;
            index_max_medico = i;
        }
    }

    printf("Medico con più visite: %s\n\n", medici[index_max_medico].id_medico);
}

void stampa_reparti(reparto *reparti, int size)
{
    for (int i = 0; i < size; i++)
    {
        reparto r = reparti[i];
        printf("Reparto %s, Turni:\n", r.id_reparto);
        for (int j = 0; j < N_TURNI; j++)
        {
            printf("Giorno %d:\n", j + 1);
            for (int k = 0; k < MAX_TURNI_GIORNO; k++)
            {
                singolo_turno t = r.turni[j][k];
                if (t.pazienti_correnti_1 + t.pazienti_correnti_2 > 0)
                {
                    printf("\tTurno: %d Pazienti totali: %d, Medico 1: %s, Pazienti 1: %d, Medico 2: %s, Pazienti 2: %d\n",
                           t.id_turno, t.pazienti_per_turno,
                           t.id_medico1, t.pazienti_correnti_1,
                           t.id_medico2, t.pazienti_correnti_2);
                }
            }
        }
        putchar('\n');
    }
}

#define N_CODICI_NOTI 5

void init_reparto_medico(reparto_medico *r, char *codice, char *id)
{
    strcpy(r->codice_richiesta_prestazione, codice);
    strcpy(r->id_reparto, id);
}

void init_reparti_medici(reparto_medico *repartiMedici)
{
    init_reparto_medico(&(repartiMedici[0]), "oculistica_avanzata", "OCT");
    init_reparto_medico(&(repartiMedici[1]), "oculistica_base", "OCT");
    init_reparto_medico(&(repartiMedici[2]), "oculistica_pre_operatoria", "OCT");
    init_reparto_medico(&(repartiMedici[3]), "ortopedia_controllo", "ORT");
    init_reparto_medico(&(repartiMedici[4]), "ortopedia_lastra", "ORT");
}

void stampa_reparti_medici(reparto_medico *repartiMedici, int size)
{
    for (int i = 0; i < size; i++)
    {
        reparto_medico r = repartiMedici[i];
        printf("Codice: %s, ID: %s\n", r.codice_richiesta_prestazione, r.id_reparto);
    }
    putchar('\n');
}

int main()
{
    srand(time(NULL));

    reparto_medico repartiMedici[N_REPARTI];
    init_reparti_medici(repartiMedici);
    puts_bold("Reparti Medici:");
    stampa_reparti_medici(repartiMedici, N_REPARTI);

    reparto *reparti = NULL;
    int size_reparti = 0;
    reparto *esuberi = NULL;
    int size_esuberi = 0;
    leggi_appuntamenti(FILENAME, repartiMedici, N_REPARTI,
                       &reparti, &size_reparti, &esuberi, &size_esuberi);
    puts_bold("Reparti:");
    stampa_reparti(reparti, size_reparti);
    puts_bold("Esuberi:");
    stampa_reparti(esuberi, size_esuberi);

    puts_bold("Esuberi:");
    medici_esuberi(esuberi, size_esuberi);

    puts_bold("Occupazione:");
    occupazione_reparti(reparti, size_reparti);

    free(reparti);
    free(esuberi);
    return 0;
}