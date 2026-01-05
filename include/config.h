#ifndef CONFIG_H
#define CONFIG_H

// ============================================================
// CHIAVI IPC
// ============================================================
#define SHM_KEY 1234        // Chiave shared memory
#define SEM_KEY 5678        // Chiave semaforo
#define MSG_KEY 9012        // Chiave coda messaggi

// ============================================================
// PARAMETRI SIMULAZIONE
// ============================================================
#define NUM_OPERATORI 4     // Numero operatori
#define NUM_UTENTI 4        // Numero utenti per test

// ============================================================
// TIPI MESSAGGIO
// ============================================================
#define MSG_TYPE_ORDINE 1           // Tipo messaggio ordine
#define MSG_TYPE_RISPOSTA_BASE 100  // Base per risposte (100 + id_utente)

// ============================================================
// TEMPI SIMULAZIONE (secondi)
// ============================================================
#define TEMPO_PREPARAZIONE 15        // Tempo preparazione piatto
#define TEMPO_CONSUMO 2        // Tempo consumazione pasto
#define TEMPO_ARRIVO 1              // Intervallo arrivi utenti


#define MSG_TYPE_ORDINE 1
#define MSG_TYPE_RISPOSTA 2


#define MAX_PIATTI_PER_CATEGORIA 20
#define FILE_PIATTI "config/piatti.txt"

typedef enum {
    CAT_PRIMI = 0,
    CAT_SECONDI = 1,
    CAT_CONTORNI = 2,    // ← AGGIUNTO
    CAT_DOLCI = 3,       // ← Indice cambiato
    CAT_CAFFE = 4,       // ← Indice cambiato
    NUM_CATEGORIE = 5    // ← Aggiornato
} Categoria;
#endif