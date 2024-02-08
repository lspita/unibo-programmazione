#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

#define FILENAME "carriera.txt"
#define LEN_NOME_COGNOME_STUDENTE 100
#define MAX_ESAMI 25
#define LEN_ID_ESAME 15
#define LEN_ESITO 3

#define LODE "30L"
#define RESPINTO "I"
#define RITIRATO "R"
#define SUFFICIENZA 18

#define ESAME_STATISTICHE "INF100" // esame usato per statistiche_esame

#define REALLOC_JUMP 5 // ogni quanti elementi fare realloc

typedef struct Esame {
    char id_esame[LEN_ID_ESAME + 1];
    char esito[LEN_ESITO + 1];
} Esame;

typedef struct Studente {
    int matricola;
    char nome_e_cognome[LEN_NOME_COGNOME_STUDENTE + 1];
    Esame esami[MAX_ESAMI];
    int n_esami; // numero reale di esami

    struct Studente *next;
} Studente;

typedef struct PianoStudente
{
    int matricola;
    char esami[MAX_ESAMI][LEN_ID_ESAME + 1];
    int n_esami; // numero reale di esami
} PianoStudente;

// inserimento unico
int push_id_esame(char *val, char (*vettore)[LEN_ID_ESAME + 1], int *size_vettore) {
    int size = *size_vettore;
    // cerca se già presente
    for (int i = 0; i < size; i++)
    {
        char *other = vettore[i];
        if (strcmp(val, other) == 0) {
            // già presente
            return i;
        }
    }
    // aggiungi nuovo esame
    assert(size < MAX_ESAMI);
    strcpy(vettore[size], val);
    (*size_vettore)++;

    return size;
}

// binary search
PianoStudente *find_piano_studente(int matricola, PianoStudente *vettore, int size) {
    if (size == 0) {
        return NULL;
    }
    int mid = size / 2;
    int mid_val = vettore[mid].matricola;
    if (matricola == mid_val) {
        // trovato
        return &(vettore[mid]);
    }
    if (size == 1) {
        // non più divisibile
        return NULL;
    }
    if (matricola > mid_val) {
        // cerca a destra
        int start = mid + 1;
        return find_piano_studente(matricola, &(vettore[start]), size - start);
    }
    if (matricola < mid_val) {
        // cerca a sinistra
        return find_piano_studente(matricola, &(vettore[0]), mid);
    }
}

// inserimento ordinato
int push_piano_studente(PianoStudente val, PianoStudente **vettore, int *size_vettore) {
    int size = *size_vettore;
    int index = -1;
    // cerca posizione
    for (int i = 0; i < size; i++)
    {
        PianoStudente other = (*vettore)[i];
        if (val.matricola < other.matricola) {
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
        *vettore = (PianoStudente *)realloc(*vettore, sizeof(PianoStudente) * (size + REALLOC_JUMP));
        assert(*vettore != NULL);
    }
    // sposta antecedenti
    for (int i = size; i > index; i--)
    {
        (*vettore)[i] = (*vettore)[i-1];
    }

    // aggiungi nuovo esame
    (*vettore)[index] = val;
    (*size_vettore)++;
    return index;
}

// inserimento in testa
void push_studente(Studente *val, Studente **lista) {
    val->next = *lista;
    *lista = val;
}

void leggi_carriera(
    char *filename, 
    Studente **out_studenti, PianoStudente **out_piano_studi, int *out_size_piano_studi
) 
{
    Studente *studenti = NULL;
    PianoStudente *piano_di_studi = NULL;
    int size_piano_studi = 0;
    // apri file
    FILE *file = fopen(filename, "r");
    assert(file != NULL);

    // loop file
    while (!feof(file))
    {
        // controllo riga vuota
        int matricola = -1;
        fscanf(file, "%d", &matricola);
        if (matricola == -1) {
            // riga vuota
            break;
        }
        // nuovo studente
        Studente *s = (Studente *)malloc(sizeof(Studente));
        assert(s != NULL);
        s->matricola = matricola;
        
        // concatena nome e cognome
        char nome[LEN_NOME_COGNOME_STUDENTE + 1] = {0};
        char cognome[LEN_NOME_COGNOME_STUDENTE + 1] = {0};
        fscanf(file, "%s %s", nome, cognome);
        sprintf(s->nome_e_cognome, "%s %s", nome, cognome);
        
        // prendi piano studi
        PianoStudente ps;
        ps.matricola = s->matricola;
        ps.n_esami = 0;
        
        // leggi esami
        s->n_esami = 0;
        for (int i = 0; i < MAX_ESAMI; i++)
        {
            char c = fgetc(file);
            if (c == ' ') {
                Esame e;
                // nuova coppia
                fscanf(file, "%s %s", e.id_esame, e.esito);
                s->esami[i] = e;
                s->n_esami++;
                // aggiungi a piano studi
                push_id_esame(e.id_esame, ps.esami, &(ps.n_esami));
            }
            else break;
        }
        // aggiungi piano studi
        push_piano_studente(ps, &piano_di_studi, &size_piano_studi);
        push_studente(s, &studenti);
    }
    

    // chiudi file
    fclose(file);
    // parametri out
    *out_studenti = studenti;
    *out_piano_studi = piano_di_studi;
    *out_size_piano_studi = size_piano_studi;
}

void pulisci_lista(Studente *s) 
{
    if (s == NULL) {
        // fine lista
        return;
    }

    Studente *next = s->next;
    // libera memoria
    free(s);
    // ricorsione
    pulisci_lista(next);
}

Esame *find_esame(char *id_esame, Esame *esami, int size) {
    Esame *e = NULL;
    // cerca ultima occorenza dell'esame (quelle)
    for (int i = 0; i < size; i++)
    {
        if (strcmp(id_esame, esami[i].id_esame) == 0) {
            // seleziona esame
            e = &(esami[i]);
        }
    }
    return e;
}

void statistiche_esame(char *id_esame, Studente *studenti, 
    float *out_superato, float *out_lodi, float *out_ritirati) 
{
    float totale = 0.0;
    float superato = 0.0, lodi = 0.0, ritirati = 0.0;

    // loop studenti
    Studente *s = studenti;
    while (s != NULL)
    {
        /*
        cerca se ha l'esame
        
        Verrà considerata l'ultima valutazione avuta sullo specifico esame,
        quindi se uno studente è stato prima bocciato e poi è passato, verrà considerato che è passato
        */
        Esame *e = find_esame(id_esame, s->esami, s->n_esami);
        if (e != NULL) {
            // esame presente
            totale++;

            // controlla esito
            if (strcmp(e->esito, LODE) == 0) {
                superato++;
                lodi++;
            }
            else if (strcmp(e->esito, RITIRATO) == 0) {
                ritirati++;
            }
            else {
                int voto = atoi(e->esito);
                if (voto >= SUFFICIENZA) {
                    superato++;
                }
            }
        }

        s = s->next;
    }
    // trasforma in percentuali
    if (totale > 0) {
        superato = (superato / totale) * 100.0;
        lodi = (lodi / totale) * 100.0;
        ritirati = (ritirati / totale) * 100.0;
    }

    // output percentuali
    *out_superato = superato;
    *out_lodi = lodi;
    *out_ritirati = ritirati;
}

void stampa_piano_studi(PianoStudente *piano_studi, int size) {
    for (int i = 0; i < size; i++)
    {
        PianoStudente ps = piano_studi[i];
        // stampa dati
        printf("MAT: %d, Esami (%d):", ps.matricola, ps.n_esami);
        for (int j = 0; j < ps.n_esami; j++)
        {
            // stampa esami
            printf(" %s", ps.esami[j]);
        }
        putchar('\n');
    }
    putchar('\n');
}

void stampa_esami(Esame *esami, int size) {
    for (int i = 0; i < size; i++)
    {
        Esame e = esami[i];
        // stampa dati
        printf("Esame: %s, Esito: %s\n", e.id_esame, e.esito);
    }
    putchar('\n');    
}

void stampa_studenti(Studente *s) 
{
    if (s == NULL) {
        // fine lista
        putchar('\n');
        return;
    }
    // stampa dati
    printf("MAT: %d, Nome: %s, Esami (%d):\n",
        s->matricola, s->nome_e_cognome, s->n_esami);
    stampa_esami(s->esami, s->n_esami);
    // ricorsione
    stampa_studenti(s->next);
}

int main()
{
    Studente *studenti;
    PianoStudente *piano_di_studi;
    int size_piano_studi;
    leggi_carriera(FILENAME, &studenti, &piano_di_studi, &size_piano_studi);
    
    puts("PIANO DI STUDI:");
    stampa_piano_studi(piano_di_studi, size_piano_studi);
    puts("STUDENTI:");
    stampa_studenti(studenti);

    float superato, lodi, ritirati;
    statistiche_esame(ESAME_STATISTICHE, studenti, &superato, &lodi, &ritirati);
    printf("STATISTICHE %s: %.2f%% passati, %.2f%% lodi, %.2f%% ritirati\n",
        ESAME_STATISTICHE, superato, lodi, ritirati);

    free(piano_di_studi);
    pulisci_lista(studenti);
    return 0;
}