#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

// ERRORE: sconto int
#define FILENAME "appuntamenti.txt"
#define LEN_NOME_PAZIENTE 100
#define START_PREVENTIVO 0
#define ORE_PER_SCONTO 10 // si considera che la somma di ore di tutti gli appuntamenti del paziente superi il valore
#define PERCENTUALE_SCONTO 15.0
#define COSTO_ORARIO_DEFAULT 80.00

#define REALLOC_JUMP 5 // ogni quanti elementi fare realloc di vettori dinamici
#define MOLTIPLICATORE_SCONTO ((100.0 - PERCENTUALE_SCONTO) / 100.0) // valore da moltiplicare per ottenere sconto

typedef struct Appuntamento
{
    int id_medico; // salvataggio medico con cui è stato richiesto l'appuntamento
    int ora_inizio;
    int ora_fine;
} Appuntamento;


typedef struct RichiestaAppuntamenti
{
    int id_paziente;
    int id_medico;
    char nome_e_cognome[LEN_NOME_PAZIENTE + 1];
    float preventivo;
    Appuntamento *appuntamenti;
    int size_appuntamenti;
} RichiestaAppuntamenti;

typedef struct Paziente
{
    int id_paziente;
    char nome_e_cognome[LEN_NOME_PAZIENTE + 1];
    float preventivo;
    Appuntamento *appuntamenti;
    int size_appuntamenti;

    struct Paziente *next;
} Paziente;


typedef struct Tariffa
{
    int id_medico;
    float costo_orario;
} Tariffa;

// inserimento con priorità
int push_tariffa(Tariffa val, Tariffa **vettore, int *size_vettore) {
    int size = *size_vettore;
    int index = -1;
    // trova posizione
    for (int i = 0; i < size; i++)
    {
        Tariffa other = (*vettore)[i];
        if (val.id_medico < other.id_medico) {
            index = i;
            break;
        }
    }
    if (index == -1) {
        // in coda
        index = size;
    }
    
    
    // aggiungi spazio se necessario
    if (size % REALLOC_JUMP == 0) {
        *vettore = (Tariffa *)realloc(*vettore, sizeof(Tariffa) * (size + REALLOC_JUMP));
        assert(*vettore != NULL);
    }
    // sposta antecedenti index
    for (int i = size; i > index; i--) {
        (*vettore)[i] = (*vettore)[i-1];
    }
    // inserisci e ritorna indice
    (*vettore)[index] = val;
    (*size_vettore)++;

    return index;
}

// inserimento vettore dinamico
int push_appuntamento(Appuntamento val, Appuntamento **vettore, int *size_vettore) {
    int size = *size_vettore;
    
    // aggiungi spazio se necessario
    if (size % REALLOC_JUMP == 0) {
        *vettore = (Appuntamento *)realloc(*vettore, sizeof(Appuntamento) * (size + REALLOC_JUMP));
        assert(*vettore != NULL);
    }

    // inserisci in coda e ritorna indice
    (*vettore)[size] = val;
    (*size_vettore)++;

    return size;
}

// inserimento in coda
void push_paziente(Paziente *val, Paziente **lista) {
    // vai in fondo
    Paziente *current = *lista;
    Paziente *prev = NULL;
    while (current != NULL)
    {
        // passa al prossimo
        prev = current;
        current = current->next;
    }
    if (prev == NULL) {
        // nuova testa
        *lista = val;
    }
    else {
        // aggiungi in coda
        prev->next = val;
    }
    
}

// ricerca paziente
Paziente *find_paziente(int id_paziente, Paziente *lista) {
    // loop lista
    while (lista != NULL)
    {
        if (lista->id_paziente == id_paziente) {
            // trovato
            return lista;
        }

        lista = lista->next; // prossimo
    }
    
    return NULL;
}

Paziente *crea_paziente(int id, char *nome, float preventivo, Appuntamento *appuntamenti, int size_appuntamenti) {
    Paziente *p = (Paziente *)malloc(sizeof(Paziente));
    assert(p != NULL);
    // assegna dati
    p->id_paziente = id;
    strcpy(p->nome_e_cognome, nome);
    p->preventivo = preventivo;
    p->appuntamenti = appuntamenti;
    p->size_appuntamenti = size_appuntamenti;

    p->next = NULL;
    return p;
}

/*
Struttura riga: Tutti gli elementi della richiesta sono divisi da spazi
Esempio (A = Appuntamento): ID_PAZIENTE ID_MEDICA NOME_COGNOME PREVENTIVO INIZIO_A1 FINE_A1 INIZIO_A2 FINE_A2 ...
*/
Paziente *leggi_appuntamenti(char *filename) {
    Paziente *pazienti = NULL;
    // apri file in lettura
    FILE *file = fopen(filename, "r");
    assert(file != NULL);

    // loop file
    while (!feof(file))
    {
        // controlla che ci siano dati
        int id_paziente = -1;
        fscanf(file, "%d", &id_paziente);
        if (id_paziente == -1) {
            // riga vuota
            break;
        }
        
        // nuova richiesta
        RichiestaAppuntamenti richiesta = {id_paziente};
        fscanf(file, "%d %s %d", &(richiesta.id_medico), richiesta.nome_e_cognome, &(richiesta.preventivo));
        richiesta.appuntamenti = NULL;
        richiesta.size_appuntamenti = 0;
        // leggi appuntamenti
        char c = '\0';
        while ((c = fgetc(file)) == ' ')
        {
            // nuova coppia
            Appuntamento a;
            fscanf(file, "%d %d", &(a.ora_inizio), &(a.ora_fine));
            a.id_medico = richiesta.id_medico;

            push_appuntamento(a, &(richiesta.appuntamenti), &(richiesta.size_appuntamenti));
        }
        
        // trova paziente interessato
        Paziente *p = find_paziente(richiesta.id_paziente, pazienti);
        if (p == NULL) {
            // nuovo paziente
            p = crea_paziente(richiesta.id_paziente, richiesta.nome_e_cognome, richiesta.preventivo, NULL, 0);
            push_paziente(p, &pazienti);
        }

        // aggiungi appuntamenti
        for (int i = 0; i < richiesta.size_appuntamenti; i++) {
            push_appuntamento(richiesta.appuntamenti[i], &(p->appuntamenti), &(p->size_appuntamenti));
        }
    }

    // chiudi file e ritorna
    fclose(file);
    return pazienti;
}

// binary search
Tariffa *find_tariffa(int id_medico, Tariffa *vettore, int size) {
    if (size == 0) {
        // vuoto
        return NULL;
    }
    
    int mid = size / 2;
    int mid_val = vettore[mid].id_medico;

    if (id_medico == mid_val) {
        // trovato
        return &(vettore[mid]);
    }
    if (size == 1) {
        // non più divisibile
        return NULL;
    }
    if (id_medico > mid_val) {
        // cerca a destra
        int start = mid + 1;
        return find_tariffa(id_medico, &(vettore[start]), size - start);
    }
    if (id_medico < mid_val) {
        // cerca a sinistra
        return find_tariffa(id_medico, &(vettore[0]), mid);
    }
}

Tariffa crea_tariffa(int id_medico, float costo) {
    // assegna dati
    Tariffa t;
    t.id_medico = id_medico;
    t.costo_orario = costo;
    return t;
}

void calcola_preventivi(Paziente *pazienti, Tariffa **tariffe, int *size_tariffe) {
    // loop lista
    Paziente *p = pazienti;
    while (p != NULL)
    {
        int ore_totali = 0;
        // loop appuntamenti
        for (int i = 0; i < p->size_appuntamenti; i++) {
            Appuntamento *a = &(p->appuntamenti[i]);
            // prendi tariffa
            Tariffa *t = find_tariffa(a->id_medico, *tariffe, *size_tariffe);
            if (t == NULL) {
                // nuova tariffa
                Tariffa nuova_tariffa = crea_tariffa(a->id_medico, COSTO_ORARIO_DEFAULT);
                int index = push_tariffa(nuova_tariffa, tariffe, size_tariffe);
                // seleziona tariffa
                t = &((*tariffe)[index]);
            }
            // aggiorna dati
            int durata = a->ora_fine - a->ora_inizio;
            p->preventivo += ((float)durata * t->costo_orario);
            ore_totali += durata;
        }

        // applica sconto
        if (ore_totali > ORE_PER_SCONTO) {
            p->preventivo *= MOLTIPLICATORE_SCONTO;
        }
        
        p = p->next; // prossimo
    }   
}

void duplica_pazienti(Paziente *p, Paziente **out_duplicati) {
    if (p == NULL) {
        // fine lista
        return;
    }

    // duplica paziente
    Paziente *d = crea_paziente(p->id_paziente, p->nome_e_cognome, p->preventivo, NULL, 0);
    // ERRORE: appuntamenti non duplicati
    for (int i = 0; i < p->size_appuntamenti; i++)
    {
        push_appuntamento(p->appuntamenti[i], &(d->appuntamenti), &(d->size_appuntamenti));
    }
    // aggiungi in lista
    push_paziente(d, out_duplicati);

    // ricorsione
    duplica_pazienti(p->next, out_duplicati);
}

void free_pazienti(Paziente *p) {
    // loop lista
    while (p != NULL)
    {
        Paziente *next = p->next;
        // libera memoria
        free(p->appuntamenti);
        free(p);
        // prossimo
        p = next;
    }
}

// TEST
void stampa_appuntamenti(Appuntamento *vettore, int size) {
    for (int i = 0; i < size; i++)
    {
        Appuntamento a = vettore[i];
        printf("Medico: %d, Inizio: %d, Fine: %d\n", 
            a.id_medico, a.ora_inizio, a.ora_fine);
    }
    putchar('\n');
}

// TEST
void stampa_tariffe(Tariffa *vettore, int size) {
    for (int i = 0; i < size; i++)
    {
        Tariffa t = vettore[i];
        printf("Medico: %d, Costo: %.2f\n",
            t.id_medico, t.costo_orario);
    }
    putchar('\n');
}

// TEST
void stampa_pazienti(Paziente *p) {
    while (p != NULL)
    {
        printf("Paziente: %d, Nome: %s, Preventivo: %.2f, Appuntamenti (%d):\n",
            p->id_paziente, p->nome_e_cognome, p->preventivo, p->size_appuntamenti);
        stampa_appuntamenti(p->appuntamenti, p->size_appuntamenti);

        p = p->next;
    }
}


int main()
{
    // inizializza tariffe
    Tariffa *tariffe = NULL;
    int size_tariffe = 0;
    
    // TEST
    push_tariffa(((Tariffa){0, 100.0}), &tariffe, &size_tariffe);
    puts("TARIFFE:");
    stampa_tariffe(tariffe, size_tariffe);

    // funzioni
    Paziente *pazienti = leggi_appuntamenti(FILENAME);

    // TEST
    puts("PAZIENTI:");
    stampa_pazienti(pazienti);

    calcola_preventivi(pazienti, &tariffe, &size_tariffe);
    
    // TEST
    puts("NUOVE TARIFFE:");
    stampa_tariffe(tariffe, size_tariffe);
    puts("PAZIENTI PREVENTIVI:");
    stampa_pazienti(pazienti);
    
    
    Paziente *duplicati = NULL;
    duplica_pazienti(pazienti, &duplicati);
    
    // TEST
    puts("PAZIENTI DUPLICATI:");
    stampa_pazienti(duplicati);

    // libera memoria
    free(tariffe);
    free_pazienti(pazienti);
    free_pazienti(duplicati);
    return 0;
}