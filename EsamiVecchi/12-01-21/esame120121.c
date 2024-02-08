#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

// len stringa = caratteri utili + terminatore

#define FILENAME "agenda.txt"
#define LEN_NOME_COGNOME_STUDENTE 99
#define MAX_LEZIONI_STUDENTE 8
#define ORA_INIZIO_LEZIONI 9
#define ORA_FINE_LEZIONI 18

typedef struct Lezione
{
    int ora_inizio;
    int durata;
} Lezione;

typedef struct Studente
{
    int matricola;
    char nome_e_cognome[LEN_NOME_COGNOME_STUDENTE + 1];
    Lezione lezioni[MAX_LEZIONI_STUDENTE];
    int n_lezioni; // nuemro di lezioni corrente

    struct Studente *next;
} Studente;

typedef struct NuovaLezione
{
    int matricola;
    int durata;
} NuovaLezione;

void insert_lezione(Lezione l, Lezione *lezioni, int *n_lezioni)
{
    if (*n_lezioni >= MAX_LEZIONI_STUDENTE)
    {
        // spazio non disponibile
        return;
    }
    int index = -1;
    for (int i = 0; i < *n_lezioni; i++)
    {
        // cerca dove posizionare
        if (l.ora_inizio < lezioni[i].ora_inizio)
        {
            index = i; // salva indice
        }
    }
    if (index == -1)
    {
        // non trovato
        index = *n_lezioni; // inserisci in coda
    }

    // sposta tutti quelli dopo index di uno a destra
    for (int i = *n_lezioni; i > index; i--)
    {
        lezioni[i] = lezioni[i - 1];
    }
    // inserisci lezione
    lezioni[index] = l;
    // aumenta totale
    (*n_lezioni)++;
}

Studente *insert_studente(Studente *val, Studente *head)
{
    // inserisci in testa
    val->next = head;
    // ritorna nuova testa
    return val;
}

Studente *leggi_lezioni(char *filename)
{
    Studente *studenti = NULL;

    FILE *file = fopen(filename, "r"); // apri file in lettura
    assert(file != NULL);

    // loop file
    // Struttura riga: MATRICOLA NOME_COGNOME INIZIO_L1 DURATA_L1 INIZIO_L2 DURATA_L2 ...
    // Esempio riga: 1234 Nome_Cognome 9 2 15 4
    while (!feof(file))
    {
        // leggi primo dato
        int matricola = -1;
        fscanf(file, "%d", &matricola);
        if (matricola == -1)
        {
            // riga vuota / fine file letto erroneamente
            break;
        }

        // nuovo studente
        Studente *s = (Studente *)malloc(sizeof(Studente));
        assert(s != NULL);
        // assegna dati
        s->matricola = matricola;
        fscanf(file, "%s", s->nome_e_cognome);
        // leggi lezioni
        s->n_lezioni = 0;
        for (int i = 0; i < MAX_LEZIONI_STUDENTE; i++)
        {
            // leggi prossimo carattere
            char c = fgetc(file);
            if (c == ' ')
            {
                // nuova lezione
                Lezione l;
                fscanf(file, "%d %d", &(l.ora_inizio), &(l.durata));
                insert_lezione(l, s->lezioni, &(s->n_lezioni));
            }
            else
                break; // fine riga
        }

        // inserisci studente in testa
        studenti = insert_studente(s, studenti);
    }

    fclose(file); // chiudi file
    return studenti;
}

void stampa_lezioni(Lezione *lezioni, int n_lezioni)
{
    printf("Lezioni (%d):\n", n_lezioni);
    for (int i = 0; i < n_lezioni; i++)
    {
        printf("Ora inizio: %d, Durata: %dh\n",
               lezioni[i].ora_inizio, lezioni[i].durata);
    }
    putchar('\n');
}

void stampa_lista(Studente *s)
{
    if (s == NULL)
    {
        // lista finita
        putchar('\n');
        return;
    }

    // stampa dati
    printf("MAT: %d, Nome: %s, ",
           s->matricola, s->nome_e_cognome, s->n_lezioni);
    stampa_lezioni(s->lezioni, s->n_lezioni);

    stampa_lista(s->next); // ricorsione con prossimo
}

Studente *get_studente(int matricola, Studente *studenti)
{
    // cerca studente con matricola richiesta
    while (studenti != NULL)
    {
        if (studenti->matricola == matricola)
        {
            // trovato
            return studenti;
        }

        studenti = studenti->next; // passa al prossimo
    }
    return NULL; // non trovato
}

void insert_nuova_lezione(NuovaLezione nl, NuovaLezione **vettore, int *size)
{
    // aggiungi cella
    int prev_size = *size;
    *vettore = (NuovaLezione *)realloc(*vettore, sizeof(NuovaLezione) * (prev_size + 1));
    assert(*vettore != NULL);
    (*size)++;

    // inserisci nuova lezione in coda
    (*vettore)[prev_size] = nl;
}

bool inserisci_nuove_lezioni(Studente *studenti, NuovaLezione *nuove_lezioni, int size_nuove_lezioni,
                             NuovaLezione **out_non_aggiunte, int *out_size_non_aggiunte)
{
    // salva grandezza vettore non aggiunte iniziale
    int start_size_non_aggiunte = *out_size_non_aggiunte;

    // per ogni nuova lezione
    for (int i = 0; i < size_nuove_lezioni; i++)
    {
        NuovaLezione nl = nuove_lezioni[i];
        bool inserita = false;
        // prendi studente interessato
        Studente *s = get_studente(nl.matricola, studenti);
        if (s != NULL && s->n_lezioni < MAX_LEZIONI_STUDENTE) // se lo studente esiste e ha posti liberi
        {
            int fine_lezione_precedente = ORA_INIZIO_LEZIONI;
            // prepara copia lezioni con lezione fine giornata fittizia
            int size_con_fine = s->n_lezioni + 1;
            Lezione lezioni_con_fine[size_con_fine];
            for (int i = 0; i < s->n_lezioni; i++)
            {
                lezioni_con_fine[i] = s->lezioni[i];
            }
            lezioni_con_fine[s->n_lezioni] = (Lezione){ORA_FINE_LEZIONI, 0};
            for (int i = 0; i < size_con_fine; i++)
            {
                Lezione l = lezioni_con_fine[i];
                // se la lezione entra tra fine precedente e inizio corrente
                if (l.ora_inizio - fine_lezione_precedente >= nl.durata)
                {
                    // crea lezione e inserisci
                    Lezione nuova = (Lezione){fine_lezione_precedente, nl.durata};
                    insert_lezione(nuova, s->lezioni, &(s->n_lezioni));
                    // segna che è stata inserita
                    inserita = true;
                    break;
                }

                // aggiorna fine precedente
                fine_lezione_precedente = l.ora_inizio + l.durata;
            }
        }
        if (!inserita)
        {
            // aggiungi a scartate
            insert_nuova_lezione(nl, out_non_aggiunte, out_size_non_aggiunte);
        }
    }

    // ritorna true se non ci sono nuove scartate
    return start_size_non_aggiunte - (*out_size_non_aggiunte) == 0;
}

void stampa_nuove_lezioni(NuovaLezione *nuove_lezioni, int size)
{
    for (int i = 0; i < size; i++)
    {
        NuovaLezione nl = nuove_lezioni[i];
        // stampa dati nuova lezione
        printf("MAT: %d, Durata: %dh\n", nl.matricola, nl.durata);
    }
    putchar('\n');
}

void free_lista(Studente *s)
{
    if (s == NULL)
    {
        // lista finite
        return;
    }

    Studente *next = s->next; // salva prossimo
    free(s);                  // libera corrente

    free_lista(next); // ricorsione con prossimo
}

int main()
{
    // leggi e stampa studenti
    Studente *studenti = leggi_lezioni(FILENAME);
    puts("Studenti:");
    stampa_lista(studenti);

    // inizializza e stampa vettore nuove lezioni
    NuovaLezione *nuove_lezioni = NULL;
    int size_nuove_lezioni = 0;

    // riempi vettore nuove lezioni
    insert_nuova_lezione((NuovaLezione){0, 3}, &nuove_lezioni, &size_nuove_lezioni);
    insert_nuova_lezione((NuovaLezione){2, 3}, &nuove_lezioni, &size_nuove_lezioni);
    insert_nuova_lezione((NuovaLezione){2, 2}, &nuove_lezioni, &size_nuove_lezioni);
    insert_nuova_lezione((NuovaLezione){1, 5}, &nuove_lezioni, &size_nuove_lezioni); // non verrà aggiunta
    insert_nuova_lezione((NuovaLezione){3, 2}, &nuove_lezioni, &size_nuove_lezioni);

    printf("Nuove lezioni (%d):\n", size_nuove_lezioni);
    stampa_nuove_lezioni(nuove_lezioni, size_nuove_lezioni);

    // inserisci nuove lezioni
    NuovaLezione *nuove_lezioni_non_aggiunte = NULL;
    int size_nuove_lezioni_non_aggiunte = 0;
    bool inserite_tutte = inserisci_nuove_lezioni(studenti, nuove_lezioni, size_nuove_lezioni,
                                                  &nuove_lezioni_non_aggiunte, &size_nuove_lezioni_non_aggiunte);

    puts("Studenti dopo aggiunte:");
    stampa_lista(studenti);

    if (inserite_tutte)
    {
        puts("Tutte le nuove lezioni sono state inserite");
    }
    else
    {
        printf("Nuove lezioni non aggiunte (%d):\n", size_nuove_lezioni_non_aggiunte);
        stampa_nuove_lezioni(nuove_lezioni_non_aggiunte, size_nuove_lezioni_non_aggiunte);
    }

    free_lista(studenti);
    free(nuove_lezioni);
    free(nuove_lezioni_non_aggiunte);
    return 0;
}