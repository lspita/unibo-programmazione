// Ludovico Maria Spitaleri 0001114169

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

// len stringa = caratteri utili + 1
// assert: se la condizione è falsa, stampa errore e chiudi il programma

#define FIELNAME "studenti.txt"
#define STUDENTE_PAGELLA "2-145" // id studente per prepara_pagella_studente
#define TODAY "08/02/2024"       // giornata di oggi per confronto dati

#define LEN_ID_STUDENTE 12
#define LEN_ID_MATERIA 12
#define LEN_ID_PROFESSORE 12
#define LEN_DESCRIZIONE_MATERIA 80
#define LEN_NOME_COGNOME_PROFESSORE 120

// data
#define LEN_GIORNO 2
#define LEN_MESE 2
#define LEN_ANNO 4
#define LEN_DIVISORE 1 /* carattere / */
#define LEN_DATA (LEN_GIORNO + LEN_MESE + LEN_ANNO + (3 * LEN_DIVISORE))

#define REALLOC_JUMP 5 /* di quanto reallocare i vettori dinamici ogni volta */

// coppia id_materia / voto
typedef struct Esito
{
    char id_materia[LEN_ID_MATERIA + 1];
    float voto;

    struct Esito *next;
} Esito;

typedef struct Studente
{
    char id_studente[LEN_ID_STUDENTE + 1];
    Esito *esiti;

    struct Studente *next;
} Studente;

typedef struct Materia
{
    char id_materia[LEN_ID_MATERIA + 1];
    char descrizione[LEN_DESCRIZIONE_MATERIA + 1];
    char id_professore[LEN_ID_PROFESSORE + 1];
} Materia;

typedef struct Professore
{
    char id_professore[LEN_ID_PROFESSORE + 1];
    char nome_cognome[LEN_NOME_COGNOME_PROFESSORE + 1];
    char data_inizio[LEN_DATA + 1];
} Professore;

typedef struct Media
{
    char id_materia[LEN_ID_MATERIA + 1];
    float media_voti;
    int occorrenze_materia; // quante volte è comparsa la materia allo studente

    struct Media *next;
} Media;

Esito *new_esito(char *materia, float voto)
{
    Esito *val = (Esito *)malloc(sizeof(Esito));
    assert(val != NULL);
    // assegna dati
    strcpy(val->id_materia, materia);
    val->voto = voto;
    val->next = NULL;

    return val;
}

// inserimento in testa
void push_esito(Esito *val, Esito **lista)
{
    if (val == NULL)
        return;
    val->next = *lista;
    *lista = val;
}

void free_esito(Esito *val)
{
    // loop lista
    while (val != NULL)
    {
        Esito *next = val->next;
        // libera memoria
        free(val);
        // prossimo
        val = next;
    }
}

Studente *new_studente(char *id, Esito *esiti)
{
    Studente *val = (Studente *)malloc(sizeof(Studente));
    assert(val != NULL);
    // assegna dati
    strcpy(val->id_studente, id);
    val->esiti = esiti;
    val->next = NULL;

    return val;
}

// inserimento in testa
void push_studente(Studente *val, Studente **lista)
{
    if (val == NULL)
        return;
    val->next = *lista;
    *lista = val;
}

Studente *find_studente(char *id, Studente *lista)
{
    // loop lista
    Studente *s = lista;
    while (s != NULL)
    {
        if (strcmp(id, s->id_studente) == 0)
        {
            // trovato
            return s;
        }

        // prossimo
        s = s->next;
    }
    // non trovato
    return NULL;
}

void free_studente(Studente *val)
{
    // loop lista
    while (val != NULL)
    {
        Studente *next = val->next;
        // libera memoria
        free_esito(val->esiti);
        free(val);
        // prossimo
        val = next;
    }
}

Materia new_materia(char *id, char *descrizione, char *professore)
{
    Materia val;
    // assegna dati
    strcpy(val.id_materia, id);
    strcpy(val.descrizione, descrizione);
    strcpy(val.id_professore, professore);

    return val;
}

// inseirmento con priorità
int push_materia(Materia val, Materia **vettore, int *size_vettore)
{
    int size = *size_vettore;
    // cerca index dove inserire
    int index = -1;
    for (int i = 0; i < size; i++)
    {
        Materia other = (*vettore)[i];
        // id crescente
        if (strcmp(val.id_materia, other.id_materia) < 0)
        {
            // index trovata
            index = i;
            break;
        }
    }
    if (index == -1)
    {
        // in coda
        index = size;
    }
    // aggiungi spazio se necessario
    if (size % REALLOC_JUMP == 0)
    {
        *vettore = (Materia *)realloc(*vettore, sizeof(Materia) * (size + REALLOC_JUMP));
        assert(*vettore != NULL);
    }
    // sposta antecedenti index
    for (int i = size; i > index; i--)
    {
        (*vettore)[i] = (*vettore)[i - 1];
    }
    // inserisci e ritorna index
    (*vettore)[index] = val;
    (*size_vettore)++;
    return index;
}

// binary search
Materia *find_materia(char *id, Materia *vettore, int size)
{
    if (size == 0)
        return NULL;
    // prendi mezzo
    int mid = size / 2;
    int cmp = strcmp(id, vettore[mid].id_materia);

    if (cmp == 0)
    {
        // trovato
        return &(vettore[mid]);
    }
    if (size == 1)
    {
        // non più divisibile
        return NULL;
    }
    if (cmp > 0)
    {
        // cerca a destra
        int start = mid + 1;
        return find_materia(id, &(vettore[start]), size - start);
    }
    if (cmp < 0)
    {
        // cerca a sinistra
        return find_materia(id, &(vettore[0]), mid);
    }
}

Professore new_professore(char *id, char *nome, char *data)
{
    Professore val;
    // assegna dati
    strcpy(val.id_professore, id);
    strcpy(val.nome_cognome, nome);
    strcpy(val.data_inizio, data);

    return val;
}

// inseirmento con priorità
int push_professore(Professore val, Professore **vettore, int *size_vettore)
{
    int size = *size_vettore;
    // cerca index dove inserire
    int index = -1;
    for (int i = 0; i < size; i++)
    {
        Professore other = (*vettore)[i];
        // id crescente
        if (strcmp(val.id_professore, other.id_professore) < 0)
        {
            // index trovata
            index = i;
            break;
        }
    }
    if (index == -1)
    {
        // in coda
        index = size;
    }
    // aggiungi spazio se necessario
    if (size % REALLOC_JUMP == 0)
    {
        *vettore = (Professore *)realloc(*vettore, sizeof(Professore) * (size + REALLOC_JUMP));
        assert(*vettore != NULL);
    }
    // sposta antecedenti index
    for (int i = size; i > index; i--)
    {
        (*vettore)[i] = (*vettore)[i - 1];
    }
    // inserisci e ritorna index
    (*vettore)[index] = val;
    (*size_vettore)++;
    return index;
}

// binary search
Professore *find_professore(char *id, Professore *vettore, int size)
{
    if (size == 0)
        return NULL;
    // prendi mezzo
    int mid = size / 2;
    int cmp = strcmp(id, vettore[mid].id_professore);

    if (cmp == 0)
    {
        // trovato
        return &(vettore[mid]);
    }
    if (size == 1)
    {
        // non più divisibile
        return NULL;
    }
    if (cmp > 0)
    {
        // cerca a destra
        int start = mid + 1;
        return find_professore(id, &(vettore[start]), size - start);
    }
    if (cmp < 0)
    {
        // cerca a sinistra
        return find_professore(id, &(vettore[0]), mid);
    }
}

Media *new_media(char *materia, float media, int occorrenze)
{
    Media *val = (Media *)malloc(sizeof(Media));
    assert(val != NULL);
    // assegna dati
    strcpy(val->id_materia, materia);
    val->media_voti = media;
    val->occorrenze_materia = occorrenze;
    val->next = NULL;

    return val;
}

// inserimento in testa
void push_media(Media *val, Media **lista)
{
    if (val == NULL)
        return;
    val->next = *lista;
    *lista = val;
}

Media *find_media(char *materia, Media *lista)
{
    // loop lista
    Media *m = lista;
    while (m != NULL)
    {
        if (strcmp(materia, m->id_materia) == 0)
        {
            // trovato
            return m;
        }

        // prossimo
        m = m->next;
    }
    // non trovato
    return NULL;
}

void free_media(Media *val)
{
    // loop lista
    while (val != NULL)
    {
        Media *next = val->next;
        // libera memoria
        free(val);
        // prossimo
        val = next;
    }
}

Studente *leggi_file(char *filename)
{
    Studente *studenti = NULL;
    // apri file in lettura
    FILE *file = fopen(filename, "r");
    assert(file != NULL);

    // loop file
    char id_studente[LEN_ID_STUDENTE + 1] = {0};
    while (fscanf(file, "%s", id_studente) == 1)
    {
        // id letto correttamente
        // cerca studente
        Studente *s = find_studente(id_studente, studenti);
        if (s == NULL)
        {
            // nuovo studente
            s = new_studente(id_studente, NULL);
            push_studente(s, &studenti);
        }
        // leggi materie
        while (fgetc(file) == ' ')
        {
            // nuova coppia
            Esito *e = new_esito("", 0.0);
            fscanf(file, "%s %f", e->id_materia, &(e->voto));
            // aggiungi esito in lista
            push_esito(e, &(s->esiti));
        }
    }

    // chiudi file e ritorna lista
    fclose(file);
    return studenti;
}

Media *prepara_pagella_studente(char *id_studente, Studente *studenti)
{
    Media *pagella = NULL;

    // cerca studente
    Studente *s = find_studente(id_studente, studenti);
    if (s != NULL)
    {
        // studente trovato
        // loop esiti
        Esito *e = s->esiti;
        while (e != NULL)
        {
            // cerca materia in medie
            Media *m = find_media(e->id_materia, pagella);
            if (m == NULL)
            {
                // nuova materia
                m = new_media(e->id_materia, 0.0, 0);
                push_media(m, &pagella);
            }
            // aggiungi occorrenza e voto
            m->occorrenze_materia++;
            m->media_voti += e->voto;

            // prossimo
            e = e->next;
        }
    }
    // loop medie
    Media *m = pagella;
    while (m != NULL)
    {
        // trasforma in media
        m->media_voti /= (float)m->occorrenze_materia;

        // prossimo
        m = m->next;
    }

    return pagella;
}

// trasforma data in giorni
unsigned long data_to_giorni(char *data)
{
    // estrai valori
    unsigned long giorno = 0, mese = 0, anno = 0;
    sscanf(data, "%lu/%lu/%lu", &giorno, &mese, &anno);
    // trasforma anno
    unsigned long giorni_anno = anno * 365;
    // trasforma mese
    unsigned long giorni_mese = 0;
    for (unsigned long i = 1; i < mese; i++)
    {
        // aggiungi giorni corretti
        switch (i)
        {
        // febbraio
        case 2:
            giorni_mese += 24;
            break;
        // aprile, giugno, settembre, novembre
        case 4:
        case 6:
        case 9:
        case 11:
            giorni_mese += 30;
            break;

        // restanti
        default:
            giorni_mese += 31;
            break;
        }
    }

    return giorno + giorni_mese + giorni_anno;
}

unsigned long giorni_di_servizio(Professore *p)
{
    // calcola giorni delle date
    unsigned long giorni_today = data_to_giorni(TODAY);
    unsigned long giorni_prof = data_to_giorni(p->data_inizio);
    // ritorna differenza
    return giorni_today - giorni_prof;
}

void stampa_pagella_studente(
    Media *pagella,
    Materia *materie, int size_materie,
    Professore *professori, int size_professori)
{
    // loop pagella
    Media *m = pagella;
    while (m != NULL)
    {
        // cerca materia
        Materia *mat = find_materia(m->id_materia, materie, size_materie);
        if (mat == NULL)
        {
            printf("MATERIA %s INESISTENTE!\n", m->id_materia);
        }
        else
        {
            // cerca professore materia
            Professore *p = find_professore(mat->id_professore, professori, size_professori);
            if (p == NULL)
            {
                printf("PROFESSORE %s INESISTENTE!\n", mat->id_professore);
            }
            else
            {
                // calcola giorni di servizio professore
                unsigned long giorni_servizio = giorni_di_servizio(p);
                // stampa dati
                printf("Materia (%s): %s, Media esiti: %.2f, Professore (%s): %s, Giorni di Servizio (%s): %lu\n",
                       m->id_materia, mat->descrizione, m->media_voti,
                       p->id_professore, p->nome_cognome, p->data_inizio, giorni_servizio);
            }
        }

        // prossimo
        m = m->next;
    }
}

void stampa_esiti(Esito *val)
{
    // loop lista
    while (val != NULL)
    {
        // stampa dati
        printf("Materia: %s, Voto: %.2f\n", val->id_materia, val->voto);

        // prossimo
        val = val->next;
    }
}

void stampa_studente(Studente *val)
{
    // loop lista
    while (val != NULL)
    {
        // stampa dati
        printf("Studente: %s, Esiti:\n", val->id_studente);
        stampa_esiti(val->esiti);
        putchar('\n');
        // prossimo
        val = val->next;
    }
}

void stampa_professori(Professore *vettore, int size)
{
    for (int i = 0; i < size; i++)
    {
        Professore p = vettore[i];
        printf("Professore: %s, Nome: %s, Data Inizio, %s\n",
               p.id_professore, p.nome_cognome, p.data_inizio);
    }
}

void stampa_materie(Materia *vettore, int size)
{
    for (int i = 0; i < size; i++)
    {
        Materia m = vettore[i];
        printf("Materia: %s, Descrizione: %s, Professore, %s\n",
               m.id_materia, m.descrizione, m.id_professore);
    }
}

int main()
{
    Professore *professori = NULL;
    int size_professori = 0;
    // aggiungi professori di test
    Professore p;
    p = new_professore("000", "Prof Mario", "01/02/2000");
    push_professore(p, &professori, &size_professori);
    p = new_professore("001", "Prof Carlo", "18/11/2004");
    push_professore(p, &professori, &size_professori);
    p = new_professore("002", "Prof Sara", "15/12/2003");
    push_professore(p, &professori, &size_professori);

    puts("PROFESSORI:");
    stampa_professori(professori, size_professori);
    putchar('\n');

    Materia *materie = NULL;
    int size_materie = 0;
    // aggiungi materie di test
    Materia m;
    m = new_materia("1-21", "Materia 1-21", "000");
    push_materia(m, &materie, &size_materie);
    m = new_materia("1-25", "Materia 1-25", "001");
    push_materia(m, &materie, &size_materie);
    m = new_materia("1-27", "Materia 1-27", "002");
    push_materia(m, &materie, &size_materie);
    m = new_materia("1-15", "Materia 1-15", "001");
    push_materia(m, &materie, &size_materie);
    m = new_materia("1-37", "Materia 1-37", "000");
    push_materia(m, &materie, &size_materie);
    m = new_materia("1-31", "Materia 1-31", "000");
    push_materia(m, &materie, &size_materie);
    m = new_materia("1-09", "Materia 1-09", "002");
    push_materia(m, &materie, &size_materie);
    m = new_materia("1-13", "Materia 1-13", "002");
    push_materia(m, &materie, &size_materie);
    m = new_materia("1-14", "Materia 1-14", "002");
    push_materia(m, &materie, &size_materie);
    m = new_materia("1-16", "Materia 1-16", "001");
    push_materia(m, &materie, &size_materie);
    m = new_materia("1-17", "Materia 1-17", "000");
    push_materia(m, &materie, &size_materie);

    puts("MATERIE:");
    stampa_materie(materie, size_materie);
    putchar('\n');

    Studente *studenti = leggi_file(FIELNAME);
    puts("STUDENTI:");
    stampa_studente(studenti);
    putchar('\n');

    Media *pagella = prepara_pagella_studente(STUDENTE_PAGELLA, studenti);

    printf("PAGELLA %s:\n", STUDENTE_PAGELLA);
    stampa_pagella_studente(pagella, materie, size_materie, professori, size_professori);
    putchar('\n');

    free_studente(studenti);
    free_media(pagella);
    free(materie);
    free(professori);
    return 0;
}