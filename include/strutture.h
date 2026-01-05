#ifndef STRUTTURE_H
#define STRUTTURE_H

#include "config.h"

// ============================================================
// STATISTICHE IN MEMORIA CONDIVISA
// ============================================================
typedef struct {
    int utenti_serviti;         // Totale utenti serviti
    int piatti_distribuiti;     // Totale piatti distribuiti
    int simulazione_attiva;     // Flag per terminazione (1=attiva, 0=termina)
} Stats;

// ============================================================
// MESSAGGIO PER CODA MESSAGGI
// ============================================================
typedef struct {
    long mtype;                 // Tipo messaggio (obbligatorio per msgrcv/msgsnd)
    int id_utente;              // ID utente che fa l'ordine
    char piatto[50];            // Nome piatto ordinato
} Messaggio;

// ============================================================
// UNION SEMCTL (necessaria su alcuni sistemi)
// ============================================================
union semun {
    int val;                    // Valore per SETVAL
    struct semid_ds *buf;       // Buffer per IPC_STAT, IPC_SET
    unsigned short *array;      // Array per GETALL, SETALL
};

typedef struct { //Piatti Fabio
    char piatti[NUM_CATEGORIE][MAX_PIATTI_PER_CATEGORIA][64];
    int num_piatti[NUM_CATEGORIE];  // Conta per ogni categoria
} ListaPiatti;

#endif // STRUTTURE_H