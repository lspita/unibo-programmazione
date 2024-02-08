#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

#define FILENAME "appuntamenti.txt"
#define N_AGENTI 100
#define LEN_NOME_COGNOME_AGENTE 70
#define LEN_ID_AGENTE 10
#define LEN_ID_CLIENTE 10
#define LEN_CODICE_RICHIESTA 15
#define SIZE_CLASSIFICA 3 // grandezza classifica premio_agente_anno

#define NOME_AGENTE_DEFAULT "NONAME" // nome agente per nuovi agenti

typedef struct Agente {
    char nome_cognome_agente[LEN_NOME_COGNOME_AGENTE + 1];
    char id_agente[LEN_ID_AGENTE + 1];
    int n_clienti_agente;
} Agente;

typedef struct Appuntamento {
    char id_cliente[LEN_ID_CLIENTE + 1];
    char id_agente[LEN_ID_AGENTE + 1];

    struct Appuntamento *next;
} Appuntamento;

typedef struct RichiestaAppuntamento {
    Appuntamento *appuntamento;
    char codice_richiesta_prestazione[LEN_CODICE_RICHIESTA + 1];
} RichiestaAppuntamento;

typedef struct Prestazione {
    char codice_richiesta_prestazione[LEN_CODICE_RICHIESTA + 1];
    Appuntamento *appuntamenti;

    struct Prestazione *next;
} Prestazione;

Prestazione *find_prestazione(char *codice, Prestazione *prestazioni) {
    // loop lista
    Prestazione *p = prestazioni;
    while (p != NULL)
    {
        if (strcmp(codice, p->codice_richiesta_prestazione) == 0) {
            // trovato
            return p;
        }

        p = p->next; // prossimo
    }
    return NULL; // non trovato
}

Appuntamento *find_appuntamento(char *id_cliente, char *id_agente, Appuntamento *appuntamenti) {
    // loop lista
    Appuntamento *a = appuntamenti;
    while (a != NULL)
    {
        if (strcmp(id_cliente, a->id_cliente) == 0 && strcmp(id_agente, a->id_agente) == 0) {
            // trovato
            return a;
        }

        a = a->next; // prossimo
    }
    return NULL; // non trovato
}

Prestazione *new_prestazione(char *codice, Appuntamento *appuntamenti) {
    Prestazione *p = (Prestazione *)malloc(sizeof(Prestazione));
    assert(p != NULL);
    // assegna dati
    strcpy(p->codice_richiesta_prestazione, codice);
    p->appuntamenti = appuntamenti;
    p->next = NULL;

    return p;
}

Appuntamento *new_appuntamento(char *id_cliente, char *id_agente) {
    Appuntamento *a = (Appuntamento *)malloc(sizeof(Appuntamento));
    assert(a != NULL);
    // assegna dati
    strcpy(a->id_cliente, id_cliente);
    strcpy(a->id_agente, id_agente);
    a->next = NULL;

    return a;
}

// inserimento in testa
void push_prestazione(Prestazione *val, Prestazione **lista) {
    val->next = *lista;
    *lista = val;
}

// inserimento in testa
void push_appuntamento(Appuntamento *val, Appuntamento **lista) {
    val->next = *lista;
    *lista = val;
}

Agente *find_agente(char *id_agente, Agente *vettore, int size) {
    for (int i = 0; i < size; i++)
    {
        Agente *a = &(vettore[i]);
        if (strcmp(id_agente, a->id_agente) == 0) {
            // trovato
            return a;
        }
    }
    return NULL; // non trovato
}

Agente new_agente(char *nome, char *id, int n_clienti) {
    Agente a;
    strcpy(a.nome_cognome_agente, nome);
    strcpy(a.id_agente, id);
    a.n_clienti_agente = n_clienti;

    return a;
}

// inserimento in vettore dinamico
int push_agente(Agente val, Agente **vettore, int *size_vettore) {
    int size = *size_vettore;
    // aggiungi spazio se necessario
    if (size % N_AGENTI == 0) {
        *vettore = (Agente *)realloc(*vettore, sizeof(Agente) * (size + N_AGENTI));
        assert(*vettore != NULL);
    }
    // aggiungi in coda
    (*vettore)[size] = val;
    (*size_vettore)++;
    // ritorna posizione di inserimento
    return size;
}

Prestazione *leggi_appuntamenti(char *filename, Agente **agenti, int *size_agenti) {
    Prestazione *prestazioni = NULL;
    // apri file
    FILE *file = fopen(filename, "r");
    assert(file != NULL);

    // loop file
    while (!feof(file))
    {
        // controllo riga vuota
        char id_cliente[LEN_ID_CLIENTE + 1] = {0};
        fscanf(file, "%s", id_cliente);
        if (strlen(id_cliente) == 0) {
            // riga vuota
            break;
        }

        // nuova richiesta
        RichiestaAppuntamento richiesta;
        char id_agente[LEN_ID_AGENTE + 1] = {0};
        fscanf(file, "%s %s\n", id_agente, richiesta.codice_richiesta_prestazione);
        // crea appuntamento
        richiesta.appuntamento = new_appuntamento(id_cliente, id_agente);

        // prendi prestazione
        Prestazione *p = find_prestazione(richiesta.codice_richiesta_prestazione, prestazioni);
        if (p == NULL) {
            // nuova prestazione
            p = new_prestazione(richiesta.codice_richiesta_prestazione, NULL);
            push_prestazione(p, &prestazioni);
        }
        // aggiungi appuntamento
        push_appuntamento(richiesta.appuntamento, &(p->appuntamenti));

        // prendi agente
        Agente *a = find_agente(richiesta.appuntamento->id_agente, *agenti, *size_agenti);
        if (a == NULL) {
            // nuovo agente
            Agente nuovo = new_agente(NOME_AGENTE_DEFAULT, richiesta.appuntamento->id_agente, 0);
            int index = push_agente(nuovo, agenti, size_agenti);
            // seleziona agente
            a = &((*agenti)[index]);
        }

        // aggiorna dati
        a->n_clienti_agente++;
    }


    // chiudi file
    fclose(file);
    return prestazioni;
}

void free_appuntamenti(Appuntamento *a) {
    if (a == NULL) {
        // fine lista
        return;
    }

    Appuntamento *next = a->next;
    // libera memoria
    free(a);
    // ricorsione
    free_appuntamenti(next);
}

void free_prestazioni(Prestazione *p) {
    if (p == NULL) {
        // fine lista
        return;
    }

    Prestazione *next = p->next;
    // libera memoria
    free_appuntamenti(p->appuntamenti);
    free(p);
    // ricorsione
    free_prestazioni(next);
}

void push_agente_classifica(Agente agente, Agente *classifica, int size) {
    int index = -1;
    // cerca posizione in classifica
    for (int i = 0; i < size; i++)
    {
        Agente other = classifica[i];
        if (agente.n_clienti_agente >= other.n_clienti_agente) {
            // posizione trovata
            index = i;
            break;
        }
    }
    if (index == -1) {
        // non in classifica
        return;
    }
    // sposta antecedenti
    for (int i = size - 1; i > index; i--) {
        classifica[i] = classifica[i - 1];
    }
    // inserisci
    classifica[index] = agente;
}

void scorri_vettore(Agente *agenti, int size_agenti, int current) {
    if (current == size_agenti) {
        // fine vettore
        putchar('\n');
        return;
    }

    // stampa dati
    Agente a = agenti[current];
    printf("Agente: %s, Nome: %s, Clienti: %d\n",
    a.id_agente, a.nome_cognome_agente, a.n_clienti_agente);

    // ricorsione
    scorri_vettore(agenti, size_agenti, current + 1);
}

void stampa_appuntamenti(Appuntamento *a) {
    if (a == NULL) {
        // fine lista
        putchar('\n');
        return;
    }

    // stampa dati
    printf("Cliente: %s, Agente: %s\n", a->id_cliente, a->id_agente);

    // ricorsione
    stampa_appuntamenti(a->next);
}

void premio_agente_anno(Prestazione *prestazioni, Agente *agenti, int size_agenti) {
    // copia vettori agenti con numero clienti unici
    Agente *agenti_unici = (Agente *)calloc(size_agenti, sizeof(Agente));
    assert(agenti_unici != NULL);
    // copia agenti
    for (int i = 0; i < size_agenti; i++)
    {
        agenti_unici[i] = agenti[i];
        agenti_unici[i].n_clienti_agente = 0; // 0 clienti unici in partenza
    }
    // prepara classifica
    int size_classifica = SIZE_CLASSIFICA > size_agenti ? size_agenti : SIZE_CLASSIFICA;
    Agente *classifica = (Agente *)calloc(size_classifica, sizeof(Agente));
    assert(classifica != NULL);

    Appuntamento *appuntamenti_unici = NULL;

    // loop lista
    Prestazione *p = prestazioni;
    while (p != NULL)
    {
        // loop appuntamenti
        Appuntamento *a = p->appuntamenti;
        while (a != NULL)
        {
            // prendi agente
            Agente *agente = find_agente(a->id_agente, agenti_unici, size_agenti);
            if (agente != NULL && find_appuntamento(a->id_cliente, a->id_agente, appuntamenti_unici) == NULL) {
                // nuovo cliente unico
                agente->n_clienti_agente++;
                // aggiungi appuntamento a unici
                Appuntamento *nuovo_a_unico = new_appuntamento(a->id_cliente, a->id_agente);
                push_appuntamento(nuovo_a_unico, &appuntamenti_unici);
            }

            a = a->next; // prossimo
        }
        
        p = p->next; // prossimo
    }

    // riempi classifica
    for (int i = 0; i < size_agenti; i++)
    {
        push_agente_classifica(agenti_unici[i], classifica, size_classifica);
    }
    // stampa classifica
    puts("CLASSIFICA AGENTI CLIENTI UNICI:");
    scorri_vettore(classifica, size_classifica, 0);

    // libera memoria
    free(agenti_unici);
    free(classifica);
    free_appuntamenti(appuntamenti_unici);
}

void stampa_prestazioni(Prestazione *p) {
    if (p == NULL) {
        // fine lista
        putchar('\n');
        return;
    }

    // stampa dati
    printf("Codice: %s, Appuntamenti:\n", p->codice_richiesta_prestazione);
    stampa_appuntamenti(p->appuntamenti);
    
    // ricorsione
    stampa_prestazioni(p->next);
}

int main()
{
    Agente *agenti = NULL;
    int size_agenti = 0;

    Prestazione *prestazioni = leggi_appuntamenti(FILENAME, &agenti, &size_agenti);
    puts("PRESTAZIONI:");
    stampa_prestazioni(prestazioni);

    puts("AGENTI:");
    scorri_vettore(agenti, size_agenti, 0);

    premio_agente_anno(prestazioni, agenti, size_agenti);
    
    free_prestazioni(prestazioni);
    free(agenti);
    return 0;
}