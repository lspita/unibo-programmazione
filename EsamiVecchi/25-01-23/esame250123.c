#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "partecipanti.txt"
#define LEN_ID_GIOCATORE 10
#define LEN_COD_TORNEO 3
#define MIN_LVL_TORNEO 0
#define MAX_LVL_TORNEO 48

#define CODICE_TORNEO_ISCRITTI "Wim"
#define CODICE_TORNEO_NUOVI_ISCRITTI "Rom"

typedef struct Torneo
{
    char codice_torneo[LEN_COD_TORNEO + 1];
    int livello_torneo;

    struct Torneo *next;
} Torneo;

typedef struct Giocatore
{
    char id_giocatore[LEN_ID_GIOCATORE + 1];
    Torneo *elenco_tornei;

    struct Giocatore *next;
} Giocatore;

void insert_torneo(Torneo *val, Torneo **lista)
{
    Torneo *current = *lista;
    Torneo *prev = NULL;
    // scorri lista per trovare posizione in cui inserire
    while (current != NULL)
    {
        if (strcmp(val->codice_torneo, current->codice_torneo) < 0)
        {
            // trovata posizione per ordine crescente
            break;
        }

        prev = current;
        current = current->next;
    }
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

Giocatore *crea_giocatore(char *id_giocatore, Torneo *elenco_tornei)
{
    // crea elemento
    Giocatore *g = (Giocatore *)malloc(sizeof(Giocatore));
    assert(g != NULL);
    // assegna campi
    strcpy(g->id_giocatore, id_giocatore);
    g->elenco_tornei = elenco_tornei;
    // ritorna elemento
    return g;
}

Torneo *crea_torneo(char *codice_torneo, int livello_torneo, Torneo *next)
{
    // crea elemento
    Torneo *t = (Torneo *)malloc(sizeof(Torneo));
    assert(t != NULL);
    // assegna campi
    strcpy(t->codice_torneo, codice_torneo);
    t->livello_torneo = livello_torneo;
    t->next = next;
    // ritorna elemento
    return t;
}

void insert_giocatore(Giocatore *val, Giocatore **lista)
{
    // inserimento in testa
    val->next = *lista;
    *lista = val;
}

Giocatore *leggi_partecipanti(char *filename)
{
    Giocatore *giocatori = NULL;
    // apri file in lettura
    FILE *file = fopen(filename, "r");
    assert(file != NULL);

    // loop file
    while (!feof(file))
    {
        // leggi primo dato
        char id_giocatore[LEN_ID_GIOCATORE + 1] = {0};
        fscanf(file, "%s", id_giocatore);
        if (strlen(id_giocatore) == 0)
        {
            // riga vuota
            break;
        }
        // nuovo giocatore
        Giocatore *g = crea_giocatore(id_giocatore, NULL);
        // leggi tornei
        char c = '\0';
        while ((c = fgetc(file)) == ' ')
        {
            // se c'è uno spazio c'è una nuova coppia di valori
            Torneo *t = crea_torneo("", 0, NULL);
            fscanf(file, "%s %d", t->codice_torneo, &(t->livello_torneo));
            // aggiungi torneo
            insert_torneo(t, &(g->elenco_tornei));
        }

        // inserisci giocatore
        insert_giocatore(g, &giocatori);
    }

    // chiudi file e ritorna lista
    fclose(file);
    return giocatori;
}

void free_tornei(Torneo *t)
{
    if (t == NULL)
    {
        // lista finita
        return;
    }
    // salva prossimo
    Torneo *next = t->next;
    // libera memoria
    free(t);
    // ricorsione con prossimo
    free_tornei(next);
}

void free_giocatori(Giocatore *g)
{
    if (g == NULL)
    {
        // lista finita
        return;
    }
    // salva prossimo
    Giocatore *next = g->next;
    // libera memoria
    free_tornei(g->elenco_tornei);
    free(g);
    // ricorsione con prossimo
    free_giocatori(next);
}

void stampa_tornei(Torneo *t)
{
    if (t == NULL)
    {
        // lista finita
        putchar('\n');
        return;
    }
    // stampa dati
    printf("Torneo: %s, Livello: %d\n", t->codice_torneo, t->livello_torneo);
    // ricorsione con prossimo
    stampa_tornei(t->next);
}

void stampa_giocatori(Giocatore *g)
{
    if (g == NULL)
    {
        // lista finita
        return;
    }
    // stampa dati
    printf("Giocatore: %s, Tornei:\n", g->id_giocatore);
    stampa_tornei(g->elenco_tornei);
    // ricorsione con prossimo
    stampa_giocatori(g->next);
}

int iscritti_torneo(Giocatore *giocatori, char *codice_torneo)
{
    int iscritti = 0;
    // loop giocatori
    Giocatore *g = giocatori;
    while (g != NULL)
    {
        // loop tornei
        Torneo *t = g->elenco_tornei;
        while (t != NULL)
        {
            if (strcmp(t->codice_torneo, codice_torneo) == 0)
            {
                // giocatore iscritto al torneo
                iscritti++;
                break;
            }

            t = t->next;
        }

        g = g->next;
    }
    return iscritti;
}

int nuovi_iscritti_torneo(Giocatore *giocatori, char *codice_torneo)
{
    int nuovi_iscritti = 0;
    // loop giocatori
    Giocatore *g = giocatori;
    while (g != NULL)
    {
        bool nuovo_giocatore = true;
        // loop tornei
        Torneo *t = g->elenco_tornei;
        while (t != NULL)
        {
            if (strcmp(t->codice_torneo, codice_torneo) == 0)
            {
                // ha già partecipato al torneo
                nuovo_giocatore = false;
                break;
            }

            t = t->next;
        }
        if (nuovo_giocatore)
        {
            // nuovo iscritto
            nuovi_iscritti++;
        }

        g = g->next;
    }
    return nuovi_iscritti;
}

int main()
{
    Giocatore *giocatori = leggi_partecipanti(FILENAME);
    puts("Giocatori:");
    stampa_giocatori(giocatori);

    int iscritti = iscritti_torneo(giocatori, CODICE_TORNEO_ISCRITTI);
    printf("Iscritti torneo %s: %d\n", CODICE_TORNEO_ISCRITTI, iscritti);

    int nuovi_iscritti = nuovi_iscritti_torneo(giocatori, CODICE_TORNEO_NUOVI_ISCRITTI);
    printf("Nuovi iscritti torneo %s: %d\n", CODICE_TORNEO_NUOVI_ISCRITTI, nuovi_iscritti);

    free_giocatori(giocatori);
    return 0;
}