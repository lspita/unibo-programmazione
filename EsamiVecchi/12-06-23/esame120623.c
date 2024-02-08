#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

#define FILENAME "appuntamenti.txt"

#define N_MEDICI 200
#define LEN_ID_MEDICO 12
#define LEN_ID_PAZIENTE 12
#define LEN_REPARTO_MEDICO 5
#define N_GIORNI 7
#define N_TURNI_GIORNO 8
#define MAX_PAZIENTI_TURNO /*4*/ 1
#define LEN_CODICE_RICHIESTA_PRESTAZIONE 25

#define DIVISORE_ID_REPARTO_MEDICO '-' // char per indicare inizio reparto in id_medico
#define MAX_OCCUPAZIONE_MEDICO (N_GIORNI * N_TURNI_GIORNO * MAX_PAZIENTI_TURNO) // massimo numero di pazienti per medico

typedef struct singolo_turno {
    int id_turno;
    int numero_pazienti;

    struct singolo_turno *next;
} singolo_turno;

typedef struct medico {
    char id_medico[LEN_ID_MEDICO + 1];
    int reparto; // reparto come int in quanto usato come indice nel vettore esuberi
    singolo_turno *turni[N_GIORNI];
} medico;

typedef struct esubero {
    bool vuoto; // true se non sono presenti turni in questo esubero
    singolo_turno *turni[N_GIORNI];
} esubero;

typedef struct record_file {
    char id_paziente[LEN_ID_MEDICO + 1];
    char id_medico[LEN_ID_MEDICO + 1];
    char codice_richiesta_prestazione[LEN_CODICE_RICHIESTA_PRESTAZIONE + 1];
    int turno_richiesto;
    int giorno_richiesto;
} record_file;

singolo_turno *new_turno(int id, int n_pazienti) {
    singolo_turno *val = (singolo_turno *)malloc(sizeof(singolo_turno));
    assert(val != NULL);
    // assegna dati
    val->id_turno = id;
    val->numero_pazienti = n_pazienti;
    val->next = NULL;

    return val;
}

// inserimento in testa
void push_turno(singolo_turno *val, singolo_turno **lista) {
    if (val == NULL) return;
    
    val->next = *lista;
    *lista = val;
}

singolo_turno *find_turno(int id, singolo_turno *lista) {
    // loop lista
    singolo_turno *t = lista;
    while (t != NULL)
    {   
        if (t->id_turno == id) {
            // trovato
            return t;
        }

        t = t->next; // prossimo
    }
    // non trovato
    return NULL;
}

int get_reparto(char *id) {
    char reparto_str[LEN_REPARTO_MEDICO + 1] = {0};
    bool is_reparto;

    for (int i = 0, j = 0; i < LEN_ID_MEDICO + 1; i++)
    {
        char next_c = id[i];
        if (is_reparto) {
            // parte del reparto
            reparto_str[j] = next_c;
            j++;
        }
        if (next_c == '\0') {
            // fine stringa
            break;
        }
        if (!is_reparto && next_c == DIVISORE_ID_REPARTO_MEDICO) {
            // prossimi caratteri saranno reparto
            is_reparto = true;
        }
    }
    // converti a int
    assert(strlen(reparto_str) > 0);
    return atoi(reparto_str);
}

medico new_medico(char *id) {
    medico m;
    strcpy(m.id_medico, id);
    // estrai reparto
    m.reparto = get_reparto(id);
    // imposta turni a vuoto
    for (int i = 0; i < N_GIORNI; i++)
    {
        m.turni[i] = NULL;
    }    

    return m;
}

// inserimento con priorità
int push_medico(medico val, medico **vettore, int *size_vettore) {
    int size = *size_vettore;
    // cerca posizione di inserimento
    int index = -1;
    for (int i = 0; i < size; i++)
    {
        medico other = (*vettore)[i];
        if (strcmp(val.id_medico, other.id_medico) < 0) {
            // pos trovata
            index = i;
            break;
        }
    }
    if (index == -1) {
        // in coda
        index = size;
    }
    // aggiungi spazio se necessario
    if (size % N_MEDICI == 0) {
        *vettore = (medico *)realloc(*vettore, sizeof(medico) * (size + N_MEDICI));
        assert(*vettore != NULL);
    }
    // sposta antecedenti
    for (int i = size; i > index; i--)
    {
        (*vettore)[i] = (*vettore)[i - 1];
    }
    // inserisci e ritorna pos
    (*vettore)[index] = val;
    (*size_vettore)++;
    return index;
}

// binary search
medico *find_medico(char *id, medico *vettore, int size) {
    if (size == 0) return NULL;

    int mid = size / 2;
    int cmp = strcmp(id, vettore[mid].id_medico);
    if (cmp == 0) {
        // trovato
        return &(vettore[mid]);
    }
    if (size == 1) {
        // non più divisibile
        return NULL;
    }
    if (cmp > 0) {
        // cerca a destra
        int start = mid + 1;
        return find_medico(id, &(vettore[start]), size - start);
    }
    if (cmp < 0) {
        // cerca a sinistra
        return find_medico(id, &(vettore[0]), mid);
    }
}

esubero new_esubero() {
    esubero e;
    // imposta vuoto
    e.vuoto = true;
    for (int i = 0; i < N_GIORNI; i++)
    {
        e.turni[i] = NULL;
    }
    
    return e;
}

void push_esubero(int reparto, esubero val, esubero **vettore, int *size_vettore) {
    int size = *size_vettore;
    // alloca spazio necessario
    if (size <= reparto) {
        int old_size = size;
        size = reparto + 1;
        *vettore = (esubero *)realloc(*vettore, sizeof(esubero) * (size));
        assert(*vettore != NULL);
        // aggiungi esuberi vuoti nelle posizioni
        for (int i = old_size; i <= reparto; i++)
        {
            (*vettore)[i] = new_esubero();
        }
    }
    // aggiungi esubero
    (*vettore)[reparto] = val;
    (*size_vettore) = size;
}

esubero *find_esubero(int reparto, esubero *vettore, int size) {
    if (size <= reparto) {
        // non presente
        return NULL;
    }
    // presente a index reparto
    return &(vettore[reparto]);
}

void leggi_appuntamenti(char *filename, 
    medico **out_medici, int *out_size_medici,
    esubero **out_esuberi, int *out_size_esuberi) 
{
    medico *medici = NULL;
    int size_medici = 0;
    esubero *esuberi = NULL;
    int size_esuberi = 0;
    // apri file in lettura
    FILE *file = fopen(filename, "r");
    assert(file != NULL);

    // loop file
    record_file riga;
    while (fscanf(file, "%s %s %s %d %d\n",
        riga.id_paziente, riga.id_medico, riga.codice_richiesta_prestazione,
        &(riga.turno_richiesto), &(riga.giorno_richiesto)) > 0)
    {
        // nuova riga
        // cerca medico
        medico *m = find_medico(riga.id_medico, medici, size_medici);
        if (m == NULL) {
            // nuovo medico
            medico nuovo_m = new_medico(riga.id_medico);
            int index = push_medico(nuovo_m, &medici, &size_medici);
            // seleziona medico
            m = &(medici[index]);
        }
        // prendi turni del giorno
        int giorno = riga.giorno_richiesto - 1; // -1 per diventare indice
        singolo_turno **turni_giorno = &(m->turni[giorno]);
        // cerca turno
        singolo_turno *t = find_turno(riga.turno_richiesto, *turni_giorno);
        if (t == NULL) {
            // nuovo turno
            t = new_turno(riga.turno_richiesto, 0);
            push_turno(t, turni_giorno);
        }
        // aggiungi paziente
        if (t->numero_pazienti < MAX_PAZIENTI_TURNO) {
            t->numero_pazienti++;
        }
        else {
            // esubero
            // cerca esubero reparto
            esubero *e = find_esubero(m->reparto, esuberi, size_esuberi);
            if (e == NULL) {
                // nuovo reparto
                esubero nuovo_e = new_esubero();
                push_esubero(m->reparto, nuovo_e, &esuberi, &size_esuberi);
                // seleziona esubero
                e = &(esuberi[m->reparto]);
            }
            
            // prendi turni del giorno
            singolo_turno **turni_giorno_e = &(e->turni[giorno]);
            // cerca turno esubero
            singolo_turno *t_e = find_turno(riga.turno_richiesto, *turni_giorno_e);
            if (t_e == NULL) {
                // nuovo turno
                t_e = new_turno(riga.turno_richiesto, 0);
                push_turno(t_e, turni_giorno_e);
                e->vuoto = false;
            }
            // aggiungi pazienti in esubero
            t_e->numero_pazienti++;
        }
    }
    

    // chiudi file e parametri in out
    fclose(file);
    *out_medici = medici;
    *out_size_medici = size_medici;
    *out_esuberi = esuberi;
    *out_size_esuberi = size_esuberi;
}

void free_turni(singolo_turno *t) {
    // loop lista
    while (t != NULL)
    {
        singolo_turno *next = t->next;
        // libera memoria
        free(t);

        t = next; // prossimo
    }
}

void free_medici(medico *vettore, int size) {
    static int i = 0;
    if (i == size) {
        // fine vettore
        free(vettore);
        i = 0;
        return;
    }

    // libera memoria
    for (int j = 0; j < N_GIORNI; j++)
    {
        free_turni(vettore[i].turni[j]);
    }

    // ricorsione
    i++;
    free_medici(vettore, size);
}

void free_esuberi(esubero *vettore, int size) {
    static int i = 0;
    if (i == size) {
        // fine vettore
        free(vettore);
        i = 0;
        return;
    }

    // libera memoria
    for (int j = 0; j < N_GIORNI; j++)
    {
        free_turni(vettore[i].turni[j]);
    }

    // ricorsione
    i++;
    free_esuberi(vettore, size);
}

void stampa_turni(int giorno, singolo_turno *t) {
    printf("Giorno: %d, Turni:\n", giorno);
    // loop lista
    while (t != NULL)
    {
        if (t->numero_pazienti > 0) {
            // stampa dati
            printf("\tTurno: %d, Pazienti: %d\n", t->id_turno, t->numero_pazienti);
        }

        t = t->next; // prossimo
    }
}

void stampa_medici(medico *vettore, int size) {
    static int i = 0;
    if (i == size) {
        // fine vettore
        i = 0;
        return;
    }

    medico m = vettore[i];
    // stampa dati
    printf("Medico: %s, Reparto: %d, Turni:\n", 
        m.id_medico, m.reparto);
    for (int j = 0; j < N_GIORNI; j++)
    {
        singolo_turno *turni = m.turni[j];
        if (turni != NULL) {
            // stampa giorno
            stampa_turni(j + 1, turni);
        }
    }
    putchar('\n');

    // ricorsione
    i++;
    stampa_medici(vettore, size);
}

void stampa_esuberi(esubero *vettore, int size) {
    static int i = 0;
    if (i == size) {
        // fine vettore
        i = 0;
        return;
    }

    esubero e = vettore[i];
    if (!e.vuoto) {
        // stampa dati
        printf("Reparto: %d, Turni:\n", i);
        for (int j = 0; j < N_GIORNI; j++)
        {    
            singolo_turno *turni = e.turni[j];
            if (turni != NULL) {
                // stampa giorno
                stampa_turni(j + 1, turni);
            }
        }
        putchar('\n');
    }

    // ricorsione
    i++;
    stampa_esuberi(vettore, size);
}

// struct di supporto per occupazione_reparti
typedef struct occ_reparto {
    int reparto;
    float occ_medici;
    int totale;

    struct occ_reparto *next;
} occ_reparto;

occ_reparto *new_occ_reparto(int reparto, float occ_medici, int totale) {
    occ_reparto *val = (occ_reparto *)malloc(sizeof(occ_reparto));
    assert(val != NULL);
    // assegna dati
    val->reparto = reparto;
    val->occ_medici = occ_medici;
    val->totale = totale;
    val->next = NULL;

    return val;
}

// inserimento in testa
void push_occ_reparto(occ_reparto *val, occ_reparto **lista) {
    if (val == NULL) return;

    val->next = *lista;
    *lista = val;
}

occ_reparto *find_occ_reparto(int reparto, occ_reparto *lista) {
    
    // loop lista
    occ_reparto *occ = lista;
    while (occ != NULL)
    {   
        if (occ->reparto == reparto) {
            // trovato
            return occ;
        }

        occ = occ->next; // prossimo
    }
    // non trovato
    return NULL;
}

void free_occ_reparto(occ_reparto *occ) {
    // loop lista
    while (occ != NULL)
    {
        occ_reparto *next = occ->next;
        // libera memoria
        free(occ);

        occ = next; // prossimo
    }
}

float media_occ_reparto(occ_reparto *occ) {
    // calcola percentuale occupazione
    return occ->occ_medici / ((float)occ->totale);
}

void stampa_occ_reparto(occ_reparto *occ) {
    // loop lista
    while (occ != NULL)
    {
        // stampa dati
        printf("Reparto: %d, Occupazione: %.2f%%\n", occ->reparto, media_occ_reparto(occ));

        occ = occ->next; // prossimo
    }
    putchar('\n');
}

void occupazione_reparti(medico *medici, int size_medici) {
    // lista che conterra le occupazioni di ogni reparto
    occ_reparto *occupazioni = NULL;

    // loop medici
    for (int i = 0; i < size_medici; i++)
    {
        medico m = medici[i];
        // cerca reparto
        occ_reparto *occ = find_occ_reparto(m.reparto, occupazioni);
        if (occ == NULL) {
            // nuovo reparto
            occ = new_occ_reparto(m.reparto, 0.0, 0);
            push_occ_reparto(occ, &occupazioni);
        }
        // calcola occupazione medico
        int pazienti = 0;
        for (int j = 0; j < N_GIORNI; j++)
        {
            singolo_turno *turni_giorno = m.turni[j];
            // loop turni giorno
            while (turni_giorno != NULL)
            {
                // aggiorna totale
                pazienti += turni_giorno->numero_pazienti;

                turni_giorno = turni_giorno->next; // prossimo
            }
            
        }
        // calcola percentuale 
        float percentuale_occupazione = (((float)pazienti) / ((float)MAX_OCCUPAZIONE_MEDICO)) * 100.0;
        // aggiungi a totale
        occ->occ_medici += percentuale_occupazione;
        occ->totale++;
    }

    // stampa risultati
    stampa_occ_reparto(occupazioni);
    // libera memoria
    free_occ_reparto(occupazioni);
}

int main()
{
    medico *medici = NULL;
    int size_medici = 0;
    esubero *esuberi = NULL;
    int size_esuberi = 0;
    leggi_appuntamenti(FILENAME, &medici, &size_medici, &esuberi, &size_esuberi);

    puts("MEDICI:");
    stampa_medici(medici, size_medici);
    puts("ESUBERI:");
    stampa_esuberi(esuberi, size_esuberi);
    puts("OCCUPAZIONE REPARTI:");
    occupazione_reparti(medici, size_medici);

    free_medici(medici, size_medici);
    free_esuberi(esuberi, size_esuberi);
    return 0;
}