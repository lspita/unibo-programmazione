#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

#define FILENAME "partite.txt"
#define LEN_ID_PARTITA 11
#define LEN_ID_TORNEO 3
#define LEN_NOME_GIOCATORE 20
#define N_SET 3 // numero di set in una partita

// vittorie_giocatore_torneo
#define GIOCATORE_CALCOLA_VITTORIE "Rossi"
#define TORNEO_CALCOLA_VITTORIE "Wim"
// MVP_torneo
#define TORNEO_CALCOLA_MVP "Wim"

typedef struct Partita
{
    char id_partita[LEN_ID_PARTITA + 1];
    char id_torneo[LEN_ID_TORNEO + 1];
    char giocatore1[LEN_NOME_GIOCATORE + 1];
    char giocatore2[LEN_NOME_GIOCATORE + 1];
    int ris1[N_SET];
    int ris2[N_SET];
    int n_set_giocati; // numero di set giocati realmente

    struct Partita *next;
} Partita;

void insert_partita(Partita *val, Partita **lista)
{
    Partita *current = *lista;
    Partita *prev = NULL;
    // loop lista
    while (current != NULL)
    {
        if (strcmp(val->id_partita, current->id_partita) < 0)
        {
            // trovata successivo a val
            break;
        }

        // passa al prossimo
        prev = current;
        current = current->next;
    }
    // inserisci val
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

void get_torneo_from_partita(char *id_partita, char *out_id_torneo)
{
    // prendi caratteri torneo
    strncpy(out_id_torneo, id_partita, LEN_ID_TORNEO);
    // aggiungi terminatore
    out_id_torneo[LEN_ID_TORNEO] = '\0';
}

Partita *leggi_partite(const char *filename)
{
    Partita *partite = NULL;
    // apri file in lettura
    FILE *file = fopen(filename, "r");
    assert(file != NULL);

    // loop file
    while (!feof(file))
    {
        // leggi primo dato
        char id_partita[LEN_ID_PARTITA + 1] = {0};
        fscanf(file, "%s", id_partita);
        if (strlen(id_partita) == 0)
        {
            // riga vuota
            break;
        }
        // nuova partita
        Partita *p = (Partita *)malloc(sizeof(Partita));
        assert(p != NULL);
        strcpy(p->id_partita, id_partita);
        get_torneo_from_partita(p->id_partita, p->id_torneo);
        // leggi dati rimanenti
        fscanf(file, "%s %s", p->giocatore1, p->giocatore2);
        p->n_set_giocati = 0;
        // leggi punteggi set
        for (int i = 0; i < N_SET; i++)
        {
            // 0 = non giocato
            p->ris1[i] = 0;
            p->ris2[i] = 0;
            // leggi prossimo carattere
            char c = fgetc(file);
            if (c == ' ')
            {
                // nuova coppia
                fscanf(file, "%d %d", &(p->ris1[i]), &(p->ris2[i]));
                p->n_set_giocati++;
            }
            else
            {
                // fine riga
                break;
            }
        }

        // aggiungi partita in lista
        insert_partita(p, &partite);
    }

    // chiudi file e ritorna lista
    fclose(file);
    return partite;
}

void stampa_partite(Partita *p)
{
    if (p == NULL)
    {
        // lista finita
        putchar('\n');
        return;
    }

    // stampa dati
    printf("Partita: %s, Torneo: %s, P1: %s, P2: %s, Sets (%d):",
           p->id_partita, p->id_torneo, p->giocatore1, p->giocatore2, p->n_set_giocati);
    // stampa risultati set
    for (int i = 0; i < p->n_set_giocati; i++)
    {
        printf(" %d-%d", p->ris1[i], p->ris2[i]);
    }
    putchar('\n');

    // ricorsione con prossimo
    stampa_partite(p->next);
}

void free_partite(Partita *p)
{
    if (p == NULL)
    {
        // lista finita
        return;
    }

    // salva prossimo
    Partita *next = p->next;
    // libera memoria
    free(p);

    // ricorsione con prossimo
    free_partite(next);
}

void vittorie_giocatore_torneo(Partita *partite, char *id_giocatore, char *id_torneo,
                               int *out_vittorie_torneo, float *out_percentuale_vittorie)
{
    int vittorie_torneo = 0;
    float percentuale_vittorie = 0.0;
    int n_partite = 0;

    // loop partite
    Partita *p = partite;
    while (p != NULL)
    {
        // controlla se il giocatore ha giocato nella partita
        int i_giocatore = 0;
        if (strcmp(p->giocatore1, id_giocatore) == 0)
        {
            // p1
            i_giocatore = 1;
        }
        else if (strcmp(p->giocatore2, id_giocatore) == 0)
        {
            // p2
            i_giocatore = 2;
        }
        if (i_giocatore != 0)
        {
            // giocatore in partita
            n_partite++;
            int set_vinti_p1 = 0, set_vinti_p2 = 0;
            // scorri set partita
            for (int i = 0; i < p->n_set_giocati; i++)
            {
                // prendi risultati giocatori
                int ris_p1 = p->ris1[i], ris_p2 = p->ris2[i];
                // aumenta vittorie
                if (ris_p1 > ris_p2)
                {
                    set_vinti_p1++;
                }
                else if (ris_p2 > ris_p1)
                {
                    set_vinti_p2++;
                }
            }
            // controlla e segna se ha vinto
            if ((i_giocatore == 1 && set_vinti_p1 > set_vinti_p2) ||
                (i_giocatore == 2 && set_vinti_p2 > set_vinti_p1))
            {
                percentuale_vittorie += 1;
                // controlla se la partita è del torneo corretto
                if (strcmp(p->id_torneo, id_torneo) == 0)
                {
                    vittorie_torneo++;
                }
            }
        }

        p = p->next; // prossima partite
    }
    // trasforma totale in percentuale
    percentuale_vittorie = (percentuale_vittorie / (float)n_partite) * 100.0;
    // parametri in output
    *out_vittorie_torneo = vittorie_torneo;
    *out_percentuale_vittorie = percentuale_vittorie;
}

// struct di supporto per calcolare MVP_torneo
typedef struct VittorieGiocatore
{
    char giocatore[LEN_NOME_GIOCATORE + 1];
    int vittorie;

    struct VittorieGiocatore *next;
} VittorieGiocatore;

VittorieGiocatore *crea_vittoria_giocatore(char *giocatore, int vittorie)
{
    // crea nodo e assegna dati
    VittorieGiocatore *vg = (VittorieGiocatore *)malloc(sizeof(VittorieGiocatore));
    assert(vg != NULL);
    strcpy(vg->giocatore, giocatore);
    vg->vittorie = vittorie;
    return vg;
}

void insert_vittoria_giocatore(VittorieGiocatore *val, VittorieGiocatore **lista)
{
    // inserimento in testa
    val->next = *lista;
    *lista = val;
}

void free_vittorie(VittorieGiocatore *vg)
{
    if (vg == NULL)
    {
        // lista finita
        return;
    }

    // salva prossimo
    VittorieGiocatore *next = vg->next;
    // libera memoria
    free(vg);

    // ricorsione con prossimo
    free_vittorie(next);
}

VittorieGiocatore *find_vittorie_giocatore(char *giocatore, VittorieGiocatore *lista)
{
    // loop lista
    VittorieGiocatore *vg = lista;
    while (vg != NULL)
    {
        if (strcmp(vg->giocatore, giocatore) == 0)
        {
            return vg; // trovato
        }

        vg = vg->next; // prossimo nodo
    }

    return NULL; // non trovato
}

void MVP_torneo(Partita *partite, char *id_torneo, char *out_mvp)
{
    // calcola set vinti di ogni giocatore
    VittorieGiocatore *vittorie_giocatori = NULL;
    // loop partite
    Partita *p = partite;
    while (p != NULL)
    {
        // controlla se la partita è del torneo corretto
        if (strcmp(p->id_torneo, id_torneo) == 0)
        {
            // cerca giocatori nei vettori
            VittorieGiocatore *vg1 = find_vittorie_giocatore(p->giocatore1, vittorie_giocatori);
            VittorieGiocatore *vg2 = find_vittorie_giocatore(p->giocatore2, vittorie_giocatori);
            if (vg1 == NULL)
            {
                // nuovo giocatore
                vg1 = crea_vittoria_giocatore(p->giocatore1, 0);
                insert_vittoria_giocatore(vg1, &vittorie_giocatori);
            }
            if (vg2 == NULL)
            {
                // nuovo giocatore
                vg2 = crea_vittoria_giocatore(p->giocatore2, 0);
                insert_vittoria_giocatore(vg2, &vittorie_giocatori);
            }
            // scorri set partita
            for (int i = 0; i < p->n_set_giocati; i++)
            {
                // prendi risultati giocatori
                int ris_p1 = p->ris1[i], ris_p2 = p->ris2[i];
                // aumenta vittorie
                if (ris_p1 > ris_p2)
                {
                    vg1->vittorie++;
                }
                else if (ris_p2 > ris_p1)
                {
                    vg2->vittorie++;
                }
            }
        }

        p = p->next; // prossima partite
    }
    // trova chi ne ha di più
    int max = -1;
    // loop lista
    VittorieGiocatore *vg = vittorie_giocatori;
    while (vg != NULL)
    {
        if (vg->vittorie > max)
        {
            // nuovo mvp
            max = vg->vittorie;
            strcpy(out_mvp, vg->giocatore);
        }

        vg = vg->next; // prossimo nodo
    }

    // libera memoria lista
    free_vittorie(vittorie_giocatori);
}

int main()
{
    Partita *partite = leggi_partite(FILENAME);
    puts("PARTITE:");
    stampa_partite(partite);

    int vittorie_torneo;
    float percentuale_vittorie;
    vittorie_giocatore_torneo(partite, GIOCATORE_CALCOLA_VITTORIE, TORNEO_CALCOLA_VITTORIE,
                              &vittorie_torneo, &percentuale_vittorie);
    printf("Vittorie giocatore %s:\n", GIOCATORE_CALCOLA_VITTORIE);
    printf("\tTorneo %s: %d\n", TORNEO_CALCOLA_VITTORIE, vittorie_torneo);
    printf("\tPercentuale generale: %.2f%%\n\n", percentuale_vittorie);

    char id_mvp_torneo[LEN_NOME_GIOCATORE + 1] = {0};
    MVP_torneo(partite, TORNEO_CALCOLA_MVP, id_mvp_torneo);
    if (strlen(id_mvp_torneo) == 0)
    {
        printf("Nessun MVP nel torneo %s\n", TORNEO_CALCOLA_MVP);
    }
    else
    {
        printf("MVP torneo %s: %s\n", TORNEO_CALCOLA_MVP, id_mvp_torneo);
    }

    free_partite(partite);
    return 0;
}